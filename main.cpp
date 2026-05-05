#include <fmt/core.h>
#include <simdjson.h>
#include "compiled_with.h"
#include "git_hash.h"
#include <string>

int main() {
	fmt::println("Hello, World!");
	fmt::println("Compiled With: {}", COMPILED_WITH);
	fmt::println("Git: {} {}", GIT_REV, GIT_BRANCH);
	{
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

		fmt::println("STATUS:{}; MESSAGE:{}; ID_DOCUMENTO_UNICO:{}", success, message, id_documento_unico);
	}
	{
		const std::string response = R"(


[
  {
    "id_atalho": 1,
    "ds_nome_atalho": "Certidões Vencidas ou a Vencer",
    "ds_titulo_atalho": "Lista de Certidões Vencidas ou à Vencer",
    "ds_texto": "Não existem certidões vencidas ou à vencer",
    "flg_perfil": "S",
    "flg_status": "A",
    "ds_view": "v_certidao_vencida",
    "id_aplicacao_padrao": null,
    "ds_executavel": null,
    "flg_migrado": null,
    "ds_funcao": "COUNT",
    "ds_cor": "#008000",
    "ds_nome_cor": "Verde",
    "ds_cor_texto": "#FFFFFF",
    "ds_nome_cor_texto": "Branco",
    "id_item_menu": null,
    "tb_usr": "S",
    "id_usuario_logado": 1080,
    "id_unidade_adm": 2,
    "id_aplicacao_regra": null,
    "ds_executavel_regra": null,
    "flg_usuario": "S",
    "flg_unidade_adm": "N"
  }
]


		)";

		simdjson::ondemand::parser parser;
		simdjson::padded_string json(response);
		simdjson::ondemand::document doc = parser.iterate(json);
		simdjson::ondemand::array array = doc.get_array();
		simdjson::ondemand::object object = array.at(0);

		int id_usuario_logado = static_cast<int>(object.find_field("id_usuario_logado").get_int64());
		int id_unidade_adm = static_cast<int>(object.find_field("id_unidade_adm").get_int64());

		fmt::println("id_usuario_logado:{}; id_unidade_adm:{}", id_usuario_logado, id_unidade_adm);
	}
	return 0;
}
