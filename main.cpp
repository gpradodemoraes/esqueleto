#include <fmt/core.h>
#include <regex>
#include <vector>
#include "compiled_with.h"
#include "git_hash.h"

static const char *testes[] = {
	"C:\\Users\\gabriel.moraes\\AppData\\Roaming\\repo\\esqueleto\\README",
	"C:\\Users\\gabriel.moraes\\AppData\\Roaming\\repo\\esqueleto\\CMakeLists.txt",
	"C:\\Users\\gabriel.moraes\\RUN\\depositos_judiciais\\2024_11_depositos_judiciais\\depositos_judiciais-2024-12-02-"
	"144541.xlsx",
	"E:\\Users\\gabriel.moraes\\RUN\\divisao_txt\\2026-02\\CEASA_Lancamentos_Contabeis (26).txt",
	"/var/log/access.log",
	"z:/Users/gabriel.moraes/my file.txt",
	"meu arquivo.pdf",
	"Gabriel Prado de Moraes"
};

static bool extract_filename_from_path_do_the_work(const std::string *path, std::string *ext, std::string *retval) {
	static const std::regex regex_win(R"(^[A-Za-z]?:?\\(.+\\)*((.+)\.(.+))?)");
	static const std::regex regex_linux(R"(/(.+/)*((.+)\.(.+))?)");
	static const std::regex regex_no_slash(R"(^([^.]+)\.?(.*))");

	const std::regex *pointer;
	if (path->find('/') != std::string::npos) {
		pointer = &regex_linux;
	} else if (path->find('\\') != std::string::npos) {
		pointer = &regex_win;
	} else {
		pointer = &regex_no_slash;
	}

	if (std::smatch file_match; std::regex_search(*path, file_match, *pointer)) {
		if (file_match.size() > 4) {
			if (file_match[3].length())
				*retval = file_match[3];
			else
				*retval = file_match.suffix();
			if (ext != nullptr && file_match[4].length()) *ext = file_match[4];
			return true;
		}
		// if(file_match.suffix()) {
		//
		// }
		if (file_match.size() > 2) {
			if (file_match[1].length())
				*retval = file_match[1];
			else
				*retval = file_match.suffix();
			if (ext != nullptr && file_match[2].length()) *ext = file_match[2];
			return true;
		}
	}
	return false;
}

std::string extract_filename_from_path(const std::string &path, std::string &ext) {
	if (std::string retval; extract_filename_from_path_do_the_work(&path, &ext, &retval)) return retval;
	return path;
}

std::string extract_filename_from_path(const std::string &path) {
	if (std::string retval; extract_filename_from_path_do_the_work(&path, nullptr, &retval)) return retval;
	return path;
}

int main() {
	fmt::println("ARRAY SIZE: {}", sizeof(testes) / sizeof(char *));

	for (int i = 0; i < sizeof(testes) / sizeof(char *); i++) {
		fmt::println("{}|{}|=>|{}|", i, testes[i], extract_filename_from_path(std::string(testes[i])));
	}
	fmt::println("===============");
	for (int i = 0; i < sizeof(testes) / sizeof(char *); i++) {
		std::string ext;
		std::string file = extract_filename_from_path(std::string(testes[i]), ext);
		fmt::println("{}|{}|=>|{}|{}|", i, testes[i], file, ext);
	}
	return 0;
}
