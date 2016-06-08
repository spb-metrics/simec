<?php

// Órgãos do módulo de obras
define('ORGAO_SESU', 1);
define('ORGAO_SETEC', 2);
define('ORGAO_FNDE', 3);
define('ORGAO_ADM', 4);
define('ORGAO_REHUF', 5);
define('ORGAO_SANEAMENTO', 6);
define('ORGAO_AMBIENTAL', 7);
define('ORGAO_ENERGIA', 8);
define('ORGAO_TRANSPORTE', 9);
define('ORGAO_LAZER', 10);


// Constantes dos ID's dos módulos que usam o módulo de obras
define('ID_OBRAS', 15);
define('ID_PARINDIGENA', 32);

// Constantes dos tipos de forma de repasse de recursos
define('TFR_CONVENIO', 2);
define('TFR_DESCENTRALIZACAO', 3);
define('TFR_REC_PROPRIO', 4);

// Constantes das funções das entidades
define('ID_UNIVERSIDADE', 12);
define('ID_HOSPITAL', 16);
define('ID_ESCOLAS_TECNICAS', 11);
define('ID_ESCOLAS_AGROTECNICAS', 14);
define('ID_ADM', 34);
define('ID_SSP', 94);

// Constantes situação da obra
define('EM_CONSTRUCAO', 1);
define('PARALIZADA', 2);
define('FINALIZADA', 3);
define('EM_ELABORACAO_DE_PROJETOS', 4);
define('EM_LICITACAO', 5);
define('NAO_CONCLUIDA', 6);

// ids das unidades
define("ID_SSP",94);
define("ID_UNIDADEIMPLANTADORA",44);
define("ID_CAMPUS",18);
define("ID_UNED",17);
define("ID_SUPERVISIONADA",35);
define("ID_REITORIA",75);
define("ID_UNIESTADUAL",42);

// ids situações de supervisão
define( "OBRSITSUPREPOSITORIO", 1 );
define( "OBRSITSUPDISTRIBUIDA", 2 );
define( "OBRSITSUPROTAEMPRESA", 4 );
define( "OBRSITSUPVISTORIA", 	5 );

// execução orçamentária
define( "OBRAS_TIPO_EXECORC_OBRAS", 1 );
define( "OBRAS_TIPO_EXECORC_EQUIPAMENTO", 2 );

// constantes workflow (POR OBRA)
if($_SESSION['baselogin'] == 'simec_desenvolvimento'){
	define( "OBR_TIPO_DOCUMENTO_OBRA", 34 );
}else{
	define( "OBR_TIPO_DOCUMENTO_OBRA", 23 );
}
define( "OBR_TIPO_DOCUMENTO_SUPERVISAO", 45 );

define( "OBRASUPERVISAO", 		   227 );
define( "OBRAANALISEMEC", 	   	   228 );
define( "OBRADEVOLVIDOSUPERVISAO", 229 );
define( "OBRAANALISEMECCORRECAO",  230 );
define( "OBRASUPERVISAOAPROVADA",  231 );
define( "OBRAVISTORIAAPROVADA",    234 );

// constantes workflow
define( "OBR_TIPO_DOCUMENTO", 18);
define( "OBRDISTRIBUIDO", 156 );
define( "OBRREDISTRIBUIDO", 209 );
define( "OBREMDEFINROTA", 157 );
define( "OBREMAVALIAMEC", 158 );
define( "OBREMAPROVAMEC", 159 );
define( "OBREMSUPERVISAO", 159 );
define( "OBREMAVALIASUPERVMEC", 171 );
define( "OBRAVALIAFINALSAA", 172 );
define( "OBRSUPFINALIZADA", 173 );
define( "OBRREAVSUPVISAO", 239 );
define( "OBRREAJSUPVISAOEMP", 280 );
define( "OBRENVREAVALSUPMEC", 174 );
define( "OBREMSUPERVISAOIND", 240 );
define( "OBRAAVALIACAOSUPERVISAO_MEC", 241 );
define( "OBRAAJUSTESUPERVISAO_EMPRESA", 242 );
define( "OBRAREAVALIACAOSUPERVISAO_MEC", 243 );
define( "OBRASUPERVISAOAPROVADAOBRA", 244 );
define( "OBRAREAJUSTESUPERVISAO_EMPRESA", 279 );
define( "GRUPOEMSUPERVISAO", 297 );
define( "GRUPOAGUARDANDOINICIOSUPERVISAO", 216 );
define( "GRUPOLIBERADOPARASUPERVISAO", 336 );


/*
// constantes workflow (DESENV)
define( "OBR_TIPO_DOCUMENTO", 14);

define( "OBRDISTRIBUIDO", 147 );
define( "OBREMDEFINROTA", 148 );
define( "OBREMAVALIAMEC", 149 );
define( "OBREMAPROVAMEC", 150 );
*/


//constantes supervisão empresas
define('OBRSITROTADEFINIDA', 4);

// Constantes de perfis do módulo
//define('PERFIL_SUPERUSUARIO', 160);
//define('PERFIL_SUPERVISORUNIDADE', 163);
//define('PERFIL_GESTORUNIDADE', 164);
define('PERFIL_SUPERVISORMEC', 165);
define('PERFIL_ADMINISTRADOR', 166);
define('PERFIL_CONSULTAGERAL', 174);
define('PERFIL_CONSULTAESTADUAL', 177);
define('PERFIL_GESTORMEC', 162);
define('PERFIL_AUDITORINTERNO', 387);
define('PERFIL_SAA', 0);
define('PERFIL_EMPRESA', 426);
define('PERFIL_CONSULTATIPOENSINO', 230);
//define('PERFIL_CONSULTAUNIDADE', 231);

// Constantes de perfis do módulo
define('PERFIL_SUPERUSUARIO', 160);
define('PERFIL_SAMPR',425);
define('PERFIL_GESTORORGAO',552);
define('PERFIL_GESTORUNIDADE',164);
define('PERFIL_SUPERVISORPR',551);
define('PERFIL_SUPERVISORORGAO',553);
define('PERFIL_SUPERVISORUNIDADE',163);
define('PERFIL_CONSULTAPR',174);
define('PERFIL_CONSULTAORGAO',230);
define('PERFIL_CONSULTAUNIDADE',231);

// Constantes de perfis do módulo
define('ADM', 391281);
define('ADM_UNICOD', 26101);

// Tipo do Aditivo
define('ADITIVO_PRAZO',		  1);
define('ADITIVO_VALOR', 	  2);
define('ADITIVO_PRAZO_VALOR', 3);

// Tipos de Arquivo
define('TIPO_ARQUIVO_FOTO_VISTORIA', 23);

// workflow do grupo da obra
define('TPDID_GRUPO', 18);
define('ESDID_SUPERVISAO_FINALIZADA', 173);

// orgãos relacionados as obras
define('ORGID_EDUCACAO_SUPERIOR'	, 1);
define('ORGID_EDUCACAO_PROFISSIONAL', 2);
define('ORGID_EDUCACAO_BASICA'		, 3);
define('ORGID_ADMINSTRATIVO'		, 4);
define('ORGID_HOSPITAIS'			, 5); 

// chaves da api de mapas
/* http://simec-presidencia */
define('CHAVE_MAPA_DESENVOLVIMENTO','ABQIAAAA5oufaSx1PgyKsR79BVQ9MhTEDgrYndhgw2rXgHo398FD2IBCChQWVIXdSqo_dLQhbadoHukIsQpUvA');
/* http://simecpr.gov.br */
define('CHAVE_MAPA_SIMEC_PR_GOV'   ,'ABQIAAAA5oufaSx1PgyKsR79BVQ9MhTOdb5KrqL7yqHrqv7xnJ7_mCVoGRQpgYZXdR1kOIhTkOoHZt-BFWF3SA');
/* http://simecpr.websis.com.br */
define('CHAVE_MAPA_SIMECPR_WEBSIS' ,'ABQIAAAA5oufaSx1PgyKsR79BVQ9MhRDVYC8DG5t0aGo8azTkT8NLOoxLBTiYs6VX_ASdfWoE6COdf0hgye51w');
/* http://simecpr.sisgov.com.br */
define('CHAVE_MAPA_SIMECPR_SISGOV' ,'ABQIAAAA5oufaSx1PgyKsR79BVQ9MhRUZGNmBpbQ5CTHQLRra12XN2fwShQgaMkxqK6v6IJ42FnfMIDwKdBg0w');

define('BANCO_FINANCEIRO_PADRAO', 104);

//WORKFLOW SISTEMA DE MONITORAMENTO DE OBRAS
//ESTADO FLUXO DA SUPERVISÃO (INDIVIDUAL)
define('WF_ESTADO_EM_CADASTRAMENTO', 320);
define('WF_ESTADO_ENVIADO_PARA_VALIDACAO', 321);
define('WF_ESTADO_VALIDADO', 322);
//AÇÃO FLUXO DA SUPERVISÃO (INDIVIDUAL)
define('WF_ACAO_ENVIAR_VALIDACAO_ORGAO', 796);
define('WF_ACAO_ENVIAR_PARA_VALIDADO', 799);

//integração de entidade com a receita
define('INTEGRA_RECEITA', true); 
?>