function loadTestform()
{
	if (document.tbrform.curhtm.value == 'confhtm') {
		testsubmit('tbconfset');
	}
	document.intervalid = setInterval('checkincmd()',100); 
	return;
}
function exitTestform()
{
	var st = document.getElementById('teststatus');
	if((st.value == '��ͣ@@testprgsz@@����')){
		window.alert('���ڲ�����');
		return false;
	}
	hidediv('confhtm');
	clearInterval(document.intervalid);
	return true;
}
function clearhistory(){
		var ctrl;
		ctrl =  document.getElementById('history');
		if(ctrl){
			ctrl.length = 0; 
		}
}
function Updatechart(){
	var ctrl,dbase,dcri;
	ctrl =  document.getElementById('chartbase');
	if(ctrl){dbase = ctrl.value;}
	ctrl =  document.getElementById('chartvary');
	if(ctrl){dcri = ctrl.value;}
	ctrl =  document.getElementById('RTestChart1');
	if(ctrl){
		var up = dbase*1.0+5.0*dcri;
		var dn = dbase*1.0-5.0*dcri;
//		window.alert(up);
//		window.alert(dn);
//		window.alert(dbase);
//		window.alert(4.0*dcri);
		ctrl.SetAxis(up,dn,dbase,4.0*dcri);
	}
}
function Resetchart(){
	var ctrl =  document.getElementById('RTestChart1');
	if(ctrl){
		ctrl.ClearAll();
	}
}
function fillcommit(cmdpos){
	var ctrl = document.getElementById(cmdpos);
	if(ctrl){
		checkincmd();
		window.clipboardData.setData('Text','test-cmd:fillcommit:'+cmdpos+'='+ctrl.value);
	}
}
function testonce(cmdparm){
	checkincmd();
	window.clipboardData.setData('Text','test-cmd:testonce:'+cmdparm);
}
function checkincmd(){
	loadTbrform();
	if(document.tbrform.curview.value != "testhtm"){
		if(document.tbrform.curview.value != "confhtm"){
			return;
		}
	}

	var cmdname = window.clipboardData.getData('Text');

	if(cmdname){
		var cmdcopy;
		cmdcopy = cmdname;
//		window.alert(cmdname);
		if(cmdname == 'rtest:header:done')  cmdname = 'rtest:header:���Խ�����ɳɹ�';
		if(cmdname == 'rtest:header:fail')  cmdname = 'rtest:header:���Խ������ʧ��';
		if(cmdname == 'rtest:header:error') cmdname = 'rtest:header:���Գ��ִ����ȡ��';

		var cmdlst = cmdname.split(':');
		if(cmdlst.length == 3){
			if(cmdlst[0] == 'rtest'){
				window.clipboardData.clearData('Text');

				var cmdctrl = document.getElementById(cmdlst[1]);
				
				if(cmdctrl){
					cmdctrl.value = cmdlst[2];
					if((cmdlst[1] != 'display')&&(cmdlst[1] != 'header')&&(cmdlst[1] != 'switch')){
						var ctrl;
						ctrl =  document.getElementById('history');
						if(ctrl){
							var all,index;
							all = '';
							for(index=0;index < ctrl.length;index++)
							{
								if(ctrl[index].text){
									all = all+ctrl[index].text+',';
								}	
							}
							all = all+cmdlst[2]+',';
							ctrl.length = ctrl.length+1; 
							var lstarray = all.split(',');//seperate by ','
							ctrl.length = lstarray.length-1;//ignore the last ,
							for (index=0;index< ctrl.length;index++){
								ctrl[index].value=lstarray[index];
								ctrl[index].text=lstarray[index];
							}
						}
					}
					if((cmdlst[1] == 'Z3')&&(cmdlst[2] != 'NA')){
						var ctrl;
						ctrl =  document.getElementById('RTestChart1');
						if(ctrl){ctrl.AddData(cmdlst[2]);}
					}
				}
			}
			if((cmdcopy == 'rtest:header:done')||
			   (cmdcopy == 'rtest:header:fail')||
			   (cmdcopy == 'rtest:header:error')){
			   	var st = document.getElementById('teststatus');
			   	if (st) st.value = '��ʼ@@testprgsz@@����';
			}
			   
		}
	}
}
function pastecmd(cmdname){
	checkincmd();
	window.clipboardData.setData('Text','test-cmd:'+cmdname);
}
function clearall(){
	var zs,cnti;
	for(cnti=1;cnti<400;cnti++){
		zs = document.getElementById('Z'+cnti);
		if(zs)	zs.value = '';
	}
	
}
function showretest(zindex){
	var st = document.getElementById('teststatus');

		
	if((st.value == '��ͣ@@testprgsz@@����')){
		return;
	}
	var zctrl= document.getElementById('Z'+zindex);
	if(zctrl){
		if(zctrl.value == '�ز�')
			return;		
		hideretest();
		if((zctrl.value == '') || (zctrl.value == ' '))
			return;

		var rval = document.getElementById('retestval');
		var rpos = document.getElementById('retestpos');
		
		rpos.value = 'Z'+zindex;
		rval.value = zctrl.value;
		zctrl.value = '�ز�';
	}
}
function hideretest(){
	var st = document.getElementById('teststatus');
	if((st.value == '��ͣ@@testprgsz@@����')){
		return;
	}
	var rval = document.getElementById('retestval');
	var rpos = document.getElementById('retestpos');
	if((rpos.value != '')){
		var oldctrl = document.getElementById(rpos.value);
		oldctrl.value = rval.value;
		rpos.value = '';
	}
}

function testsubmit(choice)
{

	var st = document.getElementById('teststatus');
	if(choice == 'tbcancel')
		return;
	if(choice == 'tbconfset'){
		if(st.value == '��ͣ@@testprgsz@@����')
			return;
		loadconfform();
		mydiv = document.getElementById("confhtm");
		mydiv.style.top = 80;
		hidediv('testhtm');
		return;
	}
	if(choice == 'tbbegin'){
		if(st.value == '��ʼ@@testprgsz@@����'){
			var st2 = document.getElementById('retestlst');
			st2.value = '';
			clearall();
			st.value = '��ͣ@@testprgsz@@����';
			pastecmd('begin');
		}else if(st.value == '��ͣ@@testprgsz@@����'){
			pastecmd('pause');
			if(window.confirm('��<ȷ��>��������,��<ȡ��>�˳�����') == true){
				pastecmd('continue');
			}else{
				pastecmd('abort');
				st.value = '��ʼ@@testprgsz@@����';
			}
		}
	}
	if(choice  == 'retest'){
		if(st.value == '��ʼ@@testprgsz@@����'){
			var rval = document.getElementById('retestval');
			var rpos = document.getElementById('retestpos');
			
			if((rpos.value == ''))
				return;
				
			var rcmd= 'retest:'+rpos.value;
			var trval = rval.value;
			var trpos = rpos.value;
			if(window.confirm('ȷ��Ҫ�ز���') != true)
				return;
			var st2 = document.getElementById('retestlst');
			st2.value += (trpos+"="+trval+";");
				
			var st3 = document.getElementById(trpos);
			st3.value = '�ز���...';
				
			st.value = '��ͣ@@testprgsz@@����';
			pastecmd(rcmd);
			
		}
	}
}
function addzedit(zindex)
{
		document.write("<input type='text' value='' ");
		document.write(" onMouseOver=showretest('"+zindex+"')");
		document.write(" onMouseOut=hideretest()");
		document.write(" onClick=testsubmit('retest')");
		document.write(" onFocus='this.blur()' size='8' class='nbinput' ");
		document.write("id='Z"+zindex+"'>");
}
document.writeln("<table width='100%'><tr >");
document.writeln("<td><input type='text' value='' id='header' onFocus='this.blur()' class='nbheader'>");
document.writeln("<br>");
document.writeln("<button type='button' id='teststatus' onClick=testsubmit('tbbegin')>��ʼ@@testprgsz@@����</button>");
document.writeln("<font color=green>|</font>");
//document.writeln("<img name='testImagel' onClick=testsubmit('tbconfset') src='imgorg/pinon.gif' width='20' height='18' >");
document.writeln("<button type='button' onClick=testsubmit('tbconfset')>��������</button>");
document.writeln("<font color=green>|��ʷ����</font>");
document.writeln("<select id='history'></select><button type='button' onClick=clearhistory()>���</button>");
document.writeln("</td></tr><tr><td>");
