function getdatatblst()
{
	return document.dataform.tbselect;
}
function loadDataform()
{
	adddatalist();
	var jhbt = document.getElementById('jhcurset');
	jhbt.value = '@@jhsz@@';
	if (GetCookie('showdatatb') == 'false')
		datapopUp();
}
function exitDataform()
{
	return true;
}
function adddatalist()
{
	var lstctl = getdatatblst();
	var lstdata="@@datalst@@";
	var lstarray = lstdata.split(',');//seperate by ','
	lstctl.length = lstarray.length-1;//ignore the last ,
	var index=0;
	for (index=0;index< lstctl.length;index++){
		lstctl[index].value=lstarray[index];
		lstctl[index].text=lstarray[index];
	}
	lstctl.value = "@@datasz@@";
}
function datasubmit(choice)
{
	var tbsel = getdatatblst();
	var lstdata="@@datalst@@";
	//check the validaty first
	if(choice == 'tbcancel')
		return;
	if(choice == 'tbadd')
	{	//dataadd
		var newname=window.prompt('��ѡ��һ�����֣�');
		if(newname ==  null) return;
		if(newname ==  '') return;
		
		if(lstdata.search(newname) >= 0){
			alert('����('+newname+')�Ѿ�����');
			return;
		}
		
		//asign the command to the cmdtyp
		//and the newname to the jhform of oldpara
		document.dataform.cmdtype.value='data_add';
		document.dataform.datasz.value=newname;
		document.dataform.submit();
		return;
	}
	if(choice == 'tbdelete')
	{	//datadelete
		if(tbsel.value == "")	return;
		if(window.confirm('��ȷ��Ҫɾ������-'+tbsel.value+'?') == true )
		{
			document.dataform.cmdtype.value='data_delete';
			document.dataform.datasz.value = tbsel.value;
			document.dataform.submit();
			return;
		}
	}
	if ((choice == 'tbselect') && (tbsel.value == '���Խ��'))
		choice = 'tbinit';
	if(choice == 'tbinit')
	{	//dataupdate
		document.dataform.cmdtype.value='data_init';
		document.dataform.datasz.value = tbsel.value;
		document.dataform.submit();
		return;
	}
	if(choice == 'tbjhset')
	{
		showpage('jhhtm');
		return;
	}
	if(choice == 'tbcheck')
	{
		document.dataform.cmdtype.value='data_check';
		document.dataform.submit();
		return;
	}
	if(choice == 'tbcompensate')
	{
		document.dataform.cmdtype.value='data_compensate';
		document.dataform.submit();
		return;
	}
	if(choice == 'tbsave')
	{
		document.dataform.cmdtype.value='data_save';
		document.dataform.submit();
		return;
	}


	//dataselect
	if(lstdata.search(tbsel.value) >= 0)
	{
		if(tbsel.value == "")	return;
		document.dataform.cmdtype.value='data_select';
		document.dataform.datasz.value = tbsel.value;
		document.dataform.submit();
		return;
	}
}
function datapopUp(distype)
{
    newX = window.event.x + document.body.scrollLeft;

    newY = window.event.y + document.body.scrollTop;
    var tb1 = document.getElementById('dataitems');
    if(tb1.style.display == '')
    {
    	tb1.style.display = 'none';
    	SetCookie('showdatatb','false');
    }
    else
    {
	tb1.style.display = '';
	SetCookie('showdatatb','true');
    }	
}
function dqdatahidepageall()
{
    var tb1 = document.getElementById('dqtest_nb');
    if(tb1){    tb1.style.display = 'none';}
    var tb1 = document.getElementById('dqtest_wb');
    if(tb1){	tb1.style.display = 'none';}
    tb1 = document.getElementById('dqdata_nb');
    if(tb1){	tb1.style.display = 'none';}
    tb1 = document.getElementById('dqdata_wb');
    if(tb1){	tb1.style.display = 'none';}

}
function dqdatashowpageall()
{
    var tb1 = document.getElementById('dqtest_nb');
    if(tb1){tb1.style.display = '';}
    var tb1 = document.getElementById('dqtest_wb');
    if(tb1){tb1.style.display = '';}
    var tb1 = document.getElementById('dqdata_nb');
    if(tb1){tb1.style.display = '';}
    var tb1 = document.getElementById('dqdata_wb');
    if(tb1){tb1.style.display = '';}
}

function dqdatapage(pindex)
{
    dqdatahidepageall();
    var tb1 = document.getElementById(pindex);
    if(tb1){tb1.style.display = '';}
}

document.writeln("<table width='100%'><tr><td>");
document.writeln("<div id='datatbdiv' style='position:absolute;width:655px; color: #8C6D5A; zindex:0'>");
document.writeln("<form name='dataform' method='POST' action='ToHtmlCtrl'><input type='hidden' name='cmdtype'><input type='hidden' name='datasz'>");
document.writeln("       <table id='dataitems' style='' border = '0' background='texture.jpg' cellspacing='0' >  ");
document.writeln("       <tr>  <td>");
document.writeln("<a href=# ><img name='dataImage2' onClick='datapopUp()' value='tbcancel' src='imgorg/pinon.gif' width='20' height='18' ></a>");
document.writeln("      </td> ");
document.writeln(" 	<td ><select name='tbselect' onchange=datasubmit('tbselect')></select></td>  ");
document.writeln("		<td ><font color='green'>|</font></td>  ");
document.writeln("		<td ><button type='button' onclick=datasubmit('tbselect')>ԭʼ����</a></td>  ");
document.writeln("		<td ><button type='button' onclick=datasubmit('tbadd')>����Ϊ</td>  ");
document.writeln("		<td ><button type='button' onclick=datasubmit('tbdelete')>ɾ��</td>  ");
document.writeln("		<td ><button type='button' onclick=datasubmit('tbsave')>����</td>  ");
document.writeln("		<td ><font color='green'>|</font></td>");
document.writeln("		<td >У�����ã�<button id='jhcurset' type='button' onclick=datasubmit('tbjhset')>δѡ��</button>");
document.writeln("		<font color='green'>|</font>");
document.writeln("		<button type='button' onclick=datasubmit('tbcheck')>У�鴦��</button>");
document.writeln("		<button type='button' onclick=datasubmit('tbcompensate')>����ֵ</button>  ");
document.writeln("		</td>");
document.writeln("		</tr></table>");
document.writeln("</form></div>");
document.writeln("<img name='dataImagel' onClick='datapopUp()' src='imgorg/pinon.gif' width='20' height='18' >");
document.writeln("</td></tr><tr><td>");
document.writeln("<p align=center style='text-align:center' >  ");
