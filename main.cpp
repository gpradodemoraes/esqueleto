#include "compiled_with.h"
#include "git_hash.h"
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>

static const int PORT = 9090;
static const int BUFFER_SIZE = 4096;
static const char PASSWORD[] = { "megustamuchoshutdown" };

std::string extractBody(const std::string &request) {
	// Find the blank line separating headers from body
	size_t pos = request.find("\r\n\r\n");
	if (pos == std::string::npos) return "";
	return request.substr(pos + 4);
}

std::string parseBodyValue(const std::string &body, const std::string &key) {
	// Parse "key=value" from body
	std::string prefix = key + "=";
	size_t pos = body.find(prefix);
	if (pos == std::string::npos) return "";
	size_t start = pos + prefix.size();
	size_t end = body.find('&', start);
	return body.substr(start, end == std::string::npos ? std::string::npos : end - start);
}

std::string buildResponse(const std::string &message) {
	std::ostringstream oss;
	oss << "HTTP/1.1 200 OK\r\n"
		<< "Content-Type: text/plain\r\n"
		<< "Content-Length: " << message.size() << "\r\n"
		<< "Connection: close\r\n"
		<< "\r\n"
		<< message;
	return oss.str();
}

std::string buildErrorResponse(int code, const std::string &reason) {
	std::string body = reason;
	std::ostringstream oss;
	oss << "HTTP/1.1 " << code << " " << reason << "\r\n"
		<< "Content-Type: text/plain\r\n"
		<< "Content-Length: " << body.size() << "\r\n"
		<< "Connection: close\r\n"
		<< "\r\n"
		<< body;
	return oss.str();
}

bool handleClient(int clientFd) {
	bool lets_shutdown = false;
	char buffer[BUFFER_SIZE] = {};
	ssize_t bytesRead = recv(clientFd, buffer, BUFFER_SIZE - 1, 0);
	if (bytesRead <= 0) {
		close(clientFd);
		return false;
	}

	std::string request(buffer, bytesRead);
	std::string response;

	// Check it's a POST request
	if (request.substr(0, 4) != "POST") {
		response = buildErrorResponse(405, "Method Not Allowed");
	} else {
		std::string body = extractBody(request);
		std::string value = parseBodyValue(body, "body");

		if (value.empty()) {
			response = buildErrorResponse(400, "Bad Request: missing 'body' field");
		} else {
			if (value == PASSWORD) {
				response = buildResponse("Password Accepted");
				lets_shutdown = true;
			} else {
				response = buildResponse("Hello " + value);
			}
		}
	}

	send(clientFd, response.c_str(), response.size(), 0);
	close(clientFd);
	return lets_shutdown;
}

int main() {
	std::cout << "Compiled With: " << COMPILED_WITH << "\nGit: " << GIT_REV << GIT_BRANCH << "\n";
	int serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFd < 0) {
		std::cerr << "Failed to create socket\n";
		return 1;
	}

	// Allow port reuse to avoid "Address already in use" on restart
	int opt = 1;
	setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	if (bind(serverFd, (sockaddr *)&addr, sizeof(addr)) < 0) {
		std::cerr << "Bind failed\n";
		return 1;
	}

	if (listen(serverFd, 10) < 0) {
		std::cerr << "Listen failed\n";
		return 1;
	}

	std::cout << "Server listening on port " << PORT << "\n";

	while (true) {
		sockaddr_in clientAddr{};
		socklen_t clientLen = sizeof(clientAddr);
		int clientFd = accept(serverFd, (sockaddr *)&clientAddr, &clientLen);
		if (clientFd < 0) {
			std::cerr << "Accept failed\n";
			continue;
		}
		if (handleClient(clientFd)) {
			pid_t pid = fork();

			if (pid == 0) {
				// Child process
				execlp("/usr/bin/sudo", "/usr/bin/sudo", "/usr/bin/shutdown", "-h", "+1", nullptr);
				// If execlp fails:
				perror("execlp");
				_exit(1);
			} else if (pid > 0) {
				// Parent process — wait for child
				int status;
				waitpid(pid, &status, 0);
				std::cout << "Child exited with: " << WEXITSTATUS(status) << "\n";
				break;
			} else {
				perror("fork");
			}
		}
	}

	close(serverFd);
	return 0;
}

