<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>

<script language="javascript" type="text/javascript">

jQuery.noConflict();

function sublinhar(obj, bool)
{
	if( bool )
		obj.style.textDecoration = 'underline';
	else
		obj.style.textDecoration = 'none';
}

function validar(opcao, tipo, id, entid, opcao_evidencia, evidencia)
{
	var html = '';
	var observacao = '';
	var upload = '';
	
	var tpvid;
	if(tipo == 'E') tpvid = 1;
	if(tipo == 'V') tpvid = 2;
	if(tipo == 'C') tpvid = 3;

	if( opcao_evidencia == 'f' && opcao )
	{
		html = 	'<input type="hidden" id="tipo" value="'+tipo+'" />' +
		   		'<input type="hidden" id="opcao" value="'+opcao+'" />' +
		   		'<input type="hidden" id="id" value="'+id+'" />' +
		   		'<input type="hidden" id="entid" value="'+entid+'" />';
		   
		jQuery("#promptMsg").html(html);

		salvar("'"+opcao_evidencia+"'","'"+evidencia+"'");
	}
	else
	{
		if( opcao_evidencia == 't' && opcao )
		{
			upload ='<tr><td>&nbsp;</td></tr>' +
					'<tr><td>&nbsp;</td></tr>' +
					'<tr>' +
					   '<td align="right">'+evidencia+':</td>' +
					   '<td>' +
					   '<form id="file_upload_form" method="post" enctype="multipart/form-data" action="enem.php?modulo=principal/atividade_enem/upload_checklist&acao=A">' +
					   '<input type="file" id="arquivo" name="arquivo" />' +
					   '<input type="hidden" id="arq_iclid" name="arq_iclid" value="'+id+'" />' +
					   '<input type="hidden" id="arq_tpvid" name="arq_tpvid" value="'+tpvid+'" />' +
					   '<input type="hidden" id="arq_entid" name="arq_entid" value="'+entid+'" />' +
					   '</form>' +
					   '<iframe id="upload_target" name="upload_target" src="" style="width:0;height:0;border:0px solid #fff;"></iframe>' +
					   '</td>' +
				   '</tr>' +
				   '<tr><td>&nbsp;</td></tr>';
		}
		if( !opcao )
		{
			observacao = '<tr><td>&nbsp;</td></tr>' +
						 '<tr>' +
			   				'<td align="right">Observação:</td>' +
			   				'<td><textarea id="vldobservacao" cols=35" rows="5"></textarea></td>' +
			   			 '</tr>';
		}
		
		html = '<span class="fechardiv" style="cursor:pointer;float:right;font-size:8px;" onmouseover="sublinhar(this,true);" onmouseout="sublinhar(this,false);" onclick="self.close();">[x]Fechar</span>' +
			   '<table border="0">' +
			   upload +
			   observacao +
			   '<tr>' +
			   '<td colspan="2" align="center"><input type="button" value="Salvar" onclick="salvar(\''+opcao_evidencia+'\',\''+evidencia+'\');" /></td>' +
			   '</tr>' +
			   '</table>' +
			   '<input type="hidden" id="tipo" value="'+tipo+'" />' +
			   '<input type="hidden" id="opcao" value="'+opcao+'" />' +
			   '<input type="hidden" id="id" value="'+id+'" />' +
			   '<input type="hidden" id="entid" value="'+entid+'" />';
			   
		jQuery("#promptMsg").html(html);
	}
}

function salvar(opcao_evidencia,evidencia)
{
	var obs 	= document.getElementById('vldobservacao');
	var arq 	= document.getElementById('arquivo');
	var tipo 	= document.getElementById('tipo').value;
	var opcao	= document.getElementById('opcao').value;
	var id		= document.getElementById('id').value;
	var entid	= document.getElementById('entid').value;
	
	if( opcao_evidencia == 't' && opcao == 'true' )
	{
		if( arq.value == '' )
		{
			alert("O campo '"+evidencia+"' deve ser preenchido.");
			arq.focus();
			return;
		}

		document.getElementById('file_upload_form').target = 'upload_target';
		document.getElementById('file_upload_form').submit();
	}

	if( opcao == 'false' )
	{
		if( obs.value == '' )
		{
			alert("O campo 'Observação' deve ser preenchido.");
			obs.focus();
			return;
		}

		observ = obs.value;
	}
	else
	{
		observ = '';
	}
	
	jQuery("#promptMsg").html('<br /><br /><br /><img src="/imagens/carregando.gif" /><br /><br />Aguarde...');
	
	var tpvid,vldsituacao;
	var item = window.opener.document.getElementById('item_'+id+'_'+tipo);
	
	if(tipo == 'E') tpvid = 1;
	if(tipo == 'V') tpvid = 2;
	if(tipo == 'C') tpvid = 3;

	if(opcao == 'true')
		vldsituacao = 't';
	else
		vldsituacao = 'f';

	var req = new Ajax.Request('enem.php?modulo=principal/atividade_enem/listar_checklist&acao=A', 
	{
        method:     'post',
        parameters: '&ajaxValidar=1&iclid='+id+'&tpvid='+tpvid+'&vldsituacao='+vldsituacao+'&vldobservacao='+observ+'&entid='+entid+'',
        onComplete: function (res)
        {
			if( res.responseText == 'ok' )
			{
				if( vldsituacao == 't' )
				{
					item.innerHTML = '<img src="/imagens/check_checklist.png" border="0" style="cursor:pointer;width:30px;height:30px;" onclick="alterarSituacao(\''+tipo+'\', \'true\', '+id+', '+entid+', \''+opcao_evidencia+'\', \''+evidencia+'\');" />';

					jQuery("#promptMsg").html('<br /><br /><br /><span style="font-size:10px;font-weight:bold;">Dados gravados com sucesso!</span><br /><br /><input type="button" value="Fechar" onclick="window.opener.location.href=window.opener.location.href;self.close();" />');
				}
				else if( vldsituacao == 'f' )
				{
					item.innerHTML = '<img src="/imagens/erro_checklist.png" border="0" style="cursor:pointer;width:30px;height:30px;" onclick="alterarSituacao(\''+tipo+'\', \'false\', '+id+', '+entid+', \''+opcao_evidencia+'\', \''+evidencia+'\');" />';
					
					jQuery("#promptMsg").html('<br /><br /><br /><span style="font-size:10px;font-weight:bold;">Dados gravados com sucesso!</span><br /><br /><input type="button" value="Fechar" onclick="window.opener.location.href=window.opener.location.href;self.close();" />');
				}
				else
				{
					jQuery("#promptMsg").html('<br /><br /><br /><span style="font-size:10px;font-weight:bold;">Ocorreu um erro ao realizar a ação.</span><br />Tente novamente ou contate o administrador do sistema.<br /><input type="button" value="Fechar" onclick="self.close();" />');
				} 
			}
			else if( res.responseText == 'pendente' )
			{
				var txt;
				if(tipo == 'V') txt = "Ainda não foi realizada nenhuma execução neste item.";
				if(tipo == 'C') txt = "Ainda não foi realizada nenhuma validação neste item.";
				
				jQuery("#promptMsg").html('<br /><br /><br /><span style="font-size:10px;font-weight:bold;">Ocorreu um erro ao realizar a ação.<br />'+txt+'</span><br /><input type="button" value="Fechar" onclick="self.close();" />');
			}
			else
			{
				jQuery("#promptMsg").html('<br /><br /><br /><span style="font-size:10px;font-weight:bold;">Ocorreu um erro ao realizar a ação.<br />Tente novamente ou contate o administrador do sistema.</span><br /><input type="button" value="Fechar" onclick="self.close();" />');
			}
        }
  	});
}

</script>

<div id="promptMsg" style="text-align:center;background-color:#f4f4f4;font-size:10px;border:1px solid black;width:350px;height:150px;">

<?php if( $_GET['situacao'] == "nulo" ): ?>
	<span class="fechardiv" style="cursor:pointer;float:right;font-size:8px;" onmouseover="sublinhar(this,true);" onmouseout="sublinhar(this,false);" onclick="self.close();">[x]Fechar</span>
	<br /><br /><br />
	<b>Deseja validar o item <b><?=$_GET['iclid']?></b>?</b> 
	<br /><br />
	<input type="button" value="Sim" onclick="validar(true, '<?=$_GET['tipo']?>', <?=$_GET['iclid']?>, <?=$_GET['entid']?>, '<?=$_GET['opcao_evidencia']?>', '<?=$_GET['evidencia']?>');" />
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	<input type="button" value="Não" onclick="validar(false, '<?=$_GET['tipo']?>', <?=$_GET['iclid']?>, <?=$_GET['entid']?>, '<?=$_GET['opcao_evidencia']?>', '<?=$_GET['evidencia']?>');" />
<?php endif; ?>

<?php if( $_GET['situacao'] == "true" ): ?>
	<span class="fechardiv" style="cursor:pointer;float:right;font-size:8px;" onmouseover="sublinhar(this,true);" onmouseout="sublinhar(this,false);" onclick="self.close();">[x]Fechar</span>
	<br /><br /><br />
	<b>Deseja invalidar o item <b><?=$_GET['iclid']?></b>?</b>
	<br /><br />
	<input type="button" value="Sim" onclick="validar(false, '<?=$_GET['tipo']?>', <?=$_GET['iclid']?>, <?=$_GET['entid']?>, '<?=$_GET['opcao_evidencia']?>', '<?=$_GET['evidencia']?>');" />
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	<input type="button" value="Não" onclick="self.close();" />
<?php endif; ?>

<?php if( $_GET['situacao'] == "false" ): ?>
	<span class="fechardiv" style="cursor:pointer;float:right;font-size:8px;" onmouseover="sublinhar(this,true);" onmouseout="sublinhar(this,false);" onclick="self.close();">[x]Fechar</span>
	<br /><br /><br />
	<b>Deseja validar este item <b><?=$_GET['iclid']?></b>?</b>
	<br /><br />
	<input type="button" value="Sim" onclick="validar(true, '<?=$_GET['tipo']?>', <?=$_GET['iclid']?>, <?=$_GET['entid']?>, '<?=$_GET['opcao_evidencia']?>', '<?=$_GET['evidencia']?>');" />
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	<input type="button" value="Não" onclick="self.close();" />
<?php endif; ?>

</div>