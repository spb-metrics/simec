<?php
function geraInsert($tabela,$array)
{
	foreach($array as $campo => $valor){
			$arrCampos[] = $campo;
			$arrValor[] = $valor ? (strstr(strtolower(str_replace(" ","",$valor)),"(select") ? $valor : "'$valor'") : "null";
	}
	return "insert into $tabela (".implode(",",$arrCampos).") values (".implode(",",$arrValor).");";
}

$tpdid = $_GET['tpdid'];
$sql = "select
			*
		from
			workflow.tipodocumento
		where
			tpdid = $tpdid";

$arrTipoDocumento = $db->pegaLinha($sql);
if($arrTipoDocumento){
	$arrSql[0] = geraInsert("workflow.tipodocumento",$arrTipoDocumento);
	$sql = "select
			*
		from
			workflow.estadodocumento
		where
			tpdid = {$arrTipoDocumento['tpdid']}
		order by
			esdid";
	$arrEstadoDocumento = $db->carregar($sql);
	if($arrEstadoDocumento){
		$n = 1;
		foreach($arrEstadoDocumento as $estadoDocumento){
			$arrSql[$n++] = geraInsert("workflow.estadodocumento",$estadoDocumento);
			$sql = "select
						*
					from
						workflow.acaoestadodoc
					where
						(esdidorigem = {$estadoDocumento['esdid']}
							OR
						esdiddestino = {$estadoDocumento['esdid']})
					order by
						aedid";
			$arrAcaoEstadoDocumentoOrigem = $db->carregar($sql);
			if($arrAcaoEstadoDocumentoOrigem){
				$y = (integer)$n.$n;
				foreach($arrAcaoEstadoDocumentoOrigem as $acaoEstadoDocumentoOrigem){
					$arrSql[$y++] = geraInsert("workflow.acaoestadodoc",$acaoEstadoDocumentoOrigem);
					$sql = "select
								*
							from
								workflow.estadodocumentoperfil
							where
								aedid = {$acaoEstadoDocumentoOrigem['aedid']}
							order by
								aedid";
					$arrAcaoEstadoDocumentoPerfil = $db->carregar($sql);
					if($arrAcaoEstadoDocumentoPerfil){
						$z = (integer)$y.$y;
						foreach($arrAcaoEstadoDocumentoPerfil as $acaoEstadoDocumentoPerfil){
							$arrSql[$z++] = geraInsert("workflow.estadodocumentoperfil",$acaoEstadoDocumentoPerfil);
						}
					}
				}
			}

		}
	}
}

$arrSql = array_unique($arrSql);
ksort($arrSql);
if($arrSql){
	header( 'Content-type: application/sql; charset=UTF-8');
    header( 'Content-Disposition: attachment; filename=script_wf.sql');
	foreach($arrSql as $sql){
		echo $sql."\n";
	}
	die;
}