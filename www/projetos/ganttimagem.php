<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

set_time_limit( 0 );

include APPRAIZ . "includes/jpgraph/jpgraph.php";
include APPRAIZ . "includes/jpgraph/jpgraph_gantt.php";

include APPRAIZ . $_SESSION['sisdiretorio'] . "/www/_constantes.php";
include APPRAIZ . $_SESSION['sisdiretorio'] . "/www/_funcoes.php";

function pegarCorBarra( $estado )
{
	switch ( strtolower( trim( $estado ) ) )
	{
		case 'em andamento':
			return array( 'lightgreen', 'darkgreen' );
		case 'suspenso':
			return array( 'lightyellow', 'yellow' );
		case 'cancelado':
			return array( 'red', 'red' );
		case 'concluído':
			return array( 'blue', 'darkblue' );
		case 'não iniciado':
		default:
			return array( 'white', 'darkgray' );
	}
}

$atiid = $_REQUEST['atiid'] == PROJETO ? null : (integer) $_REQUEST['atiid'];
$atividades = atividade_pegar_filhas( PROJETO, $atiid );
$atividades = $atividades ? $atividades : array();
$atividade_pai = $atividades[0];

$grafico = new GanttGraph( 0, 0, 'auto' );
$grafico->SetShadow( false );
$grafico->SetMargin( 5, 250, 5, 5 );
$grafico->SetBox( true, array( 0, 0, 0 ) );
$grafico->scale->SetWeekStart( 1 );
$grafico->title->Set( $atividade_pai['numero'] . ' ' . $atividade_pai['atidescricao'] );
$grafico->scale->week->SetStyle( WEEKSTYLE_FIRSTDAY );
$grafico->scale->week->SetFont( FF_FONT1 );
$grafico->title->SetFont( FF_ARIAL, FS_BOLD, 20 );

$grafico->scale->month->SetBackgroundColor( 'black:1.7' );
$grafico->scale->year->SetBackgroundColor( 'black:1.7' ); 

$grafico->scale->tableTitle->Set( 'Atividades' );
$grafico->scale->tableTitle->SetFont( FF_ARIAL, FS_NORMAL, 12 );
$grafico->scale->SetTableTitleBackground( 'black:1.7' );
$grafico->scale->tableTitle->Show( true );

// calcula data inicial e final
$data_menor = array();
$data_maior = array();
reset( $atividades );
foreach ( $atividades as $atividade )
{
	if ( $atividade['atidatainicio'] )
	{
		array_push( $data_menor, $atividade['atidatainicio'] );
	}
	if ( $atividade['atidatafim'] )
	{
		array_push( $data_maior, $atividade['atidatafim'] );
	}
}
if ( $data_menor && $data_menor )
{
	$data_menor = min( $data_menor );
	$data_menor = $data_menor ? $data_menor : date( 'Y-m-d' );
	$data_maior = max( $data_maior );
	if ( !$data_maior )
	{
		$dados = explode( '-', $data_menor );
		$data_maior = date( 'Y-m-d', mktime( 0, 0, 0, $dados[1], $dados[2] + 3, $dados[0] ) );
	}
	
	// define detalhes das datas a partir do tamanho do período
	$dados_inicio = explode( '-', $data_menor );
	$dados_termino = explode( '-', $data_maior );
	// duração em meses
	$duracao_meses = ( $dados_termino[0] - $dados_inicio[0] ) * 12;
	if ( $dados_termino[1] > $dados_inicio[1] )
	{
		$duracao_meses += $dados_termino[1] - $dados_inicio[1];
	}
	else
	{
		$duracao_meses -= $dados_inicio[1] - $dados_termino[1];
	}
	$duracao_anos = floor( $duracao_meses / 12 );
}
else
{
	$data_menor = date( 'Y-m-d' );
	$data_maior = date( 'Y-m-d', mktime( 0, 0, 0, date( 'm' ), date( 'd' ) + 3, date( 'Y' ) ) );
	$duracao_anos = 0;
}

switch ( true )
{
	case $duracao_anos < 2;
		$grafico->ShowHeaders( GANTT_HYEAR | GANTT_HMONTH | GANTT_HDAY | GANTT_HWEEK );
		break;
	case $duracao_anos < 5;
		$grafico->ShowHeaders( GANTT_HYEAR | GANTT_HMONTH );
		break;
	default;
		$grafico->ShowHeaders( GANTT_HYEAR );
		break;
}

// calcula data para tarefas que não possuem data (data fora do range)
$dados = explode( '-', $data_menor );
$data_fora = ( $dados[0] - 1 ) . '-' . $dados[1] . '-' . $dados[2]; 

reset( $atividades );
$chave_grafico = 0;
foreach ( $atividades as $chave => $atividade )
{
	$possui_filho = $atividades[$chave+1] && $atividades[$chave+1]['profundidade'] > $atividade_pai['profundidade'];
	$profundidade = $atividade['profundidade'] - $atividade_pai['profundidade'];
	if ( $profundidade > 4 )
	{
		continue;
	}
	$concluido =  (integer) $atividade['atiporcentoexec'];
	$descricao = str_repeat( '   ', $profundidade ) . $atividade['numero'] . ' ' . $atividade['atidescricao'];
	if ( !$atividade['atidatainicio'] || !$atividade['atidatafim'] )
	{
		$inicio = $data_fora;
		$termino = $data_fora;
	}
	else
	{
		$inicio = $atividade['atidatainicio'];
		$termino = $atividade['atidatafim'];
	}
	$cor = pegarCorBarra( $atividade['esadescricao'] );
	
	$label = trim( $concluido . '% ' . $atividade['usunome'] );
	$barra = new GanttBar( $chave_grafico, $descricao, $inicio, $termino, $label, 10 );
	$barra->rightMark->SetType( MARK_FILLEDCIRCLE );
	$barra->title->SetFont( FF_FONT1, FS_NORMAL, 8 );
	$barra->title->SetColor( $cor[1] );
	$barra->SetPattern( BAND_SOLID, $cor[0], 5 );
	$barra->progress->Set( $concluido / 100 );
	$barra->progress->SetPattern( GANTT_SOLID, $cor[1] );
	$grafico->Add( $barra );
	$chave_grafico++;
}

$grafico->SetDateRange( $data_menor, $data_maior );
$grafico->SetBackgroundGradient( 'white', 'white' );
$grafico->SetFrame( false );

//exit();

$grafico->Stroke();

?>