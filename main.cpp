#include <fmt/core.h>
#include <simdjson.h>
#include "compiled_with.h"
#include "git_hash.h"
#include <string>

int main() {
	fmt::println("Hello, World!");
	fmt::println("Compiled With: {}", COMPILED_WITH);
	fmt::println("Git: {} {}", GIT_REV, GIT_BRANCH);

	int id_documento_unico = 0;
	const std::string response =
		R"({"success":true,"message":"Registro Gravado com Sucesso.","id_prestacao_contas":19789,"idEdicaoPdc":54741,"id_documento_unico":3043355,"ano":"2026","id_item_pdc":[9671789,0]})";

	simdjson::ondemand::parser parser;
	simdjson::padded_string json(response);
	simdjson::ondemand::document doc = parser.iterate(json);
	simdjson::ondemand::object object = doc.get_object();

	bool success = static_cast<bool>(object.find_field("success").get_bool());
	std::string message;
	object.find_field("message").get_string(message);
	if (success) {
		id_documento_unico = static_cast<int>(object.find_field("id_documento_unico").get_int64());
	}

	fmt::println("STATUS:{}; MESSAGE:{}; ID_DOCUMENTO_UNICO:{}", success,message,id_documento_unico);

	return 0;
}
