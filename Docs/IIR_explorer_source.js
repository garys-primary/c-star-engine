
<html>
<HEAD>
<title>IIR Filter Explorer</title>
<meta name="author" content="Jon Grove">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta name="description" content="IIR-Explorer, design tool for Infinite Impulse Response filters.">
<link rel="canonical" href="http://jaggedplanet.com/iir/iir-explorer.asp">
<STYLE>
html, body
{
	font-family: Arial, Helvetica, sans-serif;
	padding:0px;
	margin:0px;
}
html
{
	background-color: #fff;
	height:100%;
}
body
{
	min-height:100%;
}
a:link		{color: #8080ff;}
a:visited	{color: #8080ff;}
a:active	{color: #8080ff;}
a:hover		{color: #ff8080;}
a			{text-decoration: underline}

#pageheader
{
	background:#000;
	background-image:url(images/logo.png);
	background-repeat:no-repeat;
	border:0px solid #fff;
	padding:0px 0px 0px 100px;
	margin:0px;
	xbox-shadow:0px 0px 20px 5px #e8e8e8;
	color:#fff;
}
#pageheader h1
{
	margin:0px;
	padding:40px 0px 20px;
}
#pageheader h1 .subhead
{
	font-size:60%;
	margin-left:15px;
	color:#ccc;
}
#menu
{
	background:#000;
	border:0px solid #fff;
	padding:0px 0px 0px 85px;
	margin:0px 0px 15px;
	box-shadow: 0px 4px 12px -2px rgba(0,0,0,0.75);
}
#menu ul
{
	list-style: none;
	padding:0;
	margin:0;
}
#menu li
{
	display: inline;
	xfloat:left;
	margin: 0px 0px 0px -5px;
	padding: 0px 20px;
	border:0px solid #444;
	border-left-width:1px;
	border-right-width:1px;
}

#menu li a
{
	text-decoration:none;
	line-height:35px;
}
#menu .selitem
{
	background-color:#444;
}
#menu .selitem a
{
	color:#fff;
}
#topad
{
	width:160px;
	height:600px;
	float:right;
	margin:0px 0px 10px 10px;
	overflow:hidden;
}
#bottomad
{
	width:100%;
	height:100px;
	margin:15px 0px;
	overflow:hidden;
}
#maintext
{
	margin:20px 40px;
	min-height:600px;
}
img 
{
	border:0px;
}
#pagefooter
{
height:30px;
background-color:#ccc;
padding:20px;
font-size:80%;
}
#pagefooter a {color:#000;}
#pagefooter a:hover {color:#008;}

@media all and (max-width:640px)
{
	#pageheader, #menu
	{
		background-image:none;
		padding:0px 0px 0px 10px;
	}
	#pageheader h1 .subhead
	{
		margin-left:0px;
		display:block;
		clear:left;
	}
	#topad
	{
		width:100%;
		height:100px;
		margin-left:0px;
		float:none;
	}
	#maintext
	{
		margin:20px 10px;
	}
}

</STYLE>

</HEAD>

<STYLE>

#maintext
{
	overflow:hidden;
}

canvas
{
	border:1px solid #ddd;
}

.centred
{
	background-color:#fff;
	width:800px;
	margin:0px;
	border:1px solid #fff
}
.bgleft
{
	background-color:#000;
	background:url(bg.png);
	background-repeat:repeat-x;
	background-position:top left;
}
.bgright
{
	background-color:#000;
	background:url(bg.png);
	background-repeat:repeat-x;
	background-position:top right;
}

button
{
	min-width:85px;
	height:30px;
	border: 2px outset #68F;
	margin: 5px;
	color:#fff;
	font-weight:bold;
	background-color: #40F;
	border-radius: 15px;
	-moz-border-radius: 15px;
	-webkit-border-radius: 15px;
}

button:hover
{
	border: 2px outset #0FF;
	background-color: #68F;
}

button:active
{
	background-color: #BBF;
}


#maincontrols input[type="number"]
{
	font-size:12px;
	width:60px;
}
#maincontrols input[type="range"]
{
	width:150px;
	padding:0px;
}

#maincontrols, #freqresponse, #timeresponse, #plots, #code
{
	font-family:arial, sans-serif;
	font-size:12px;
	width:310px;
	height:350px;
	padding:10px;
	margin:5px;
	border:1px solid #ccc;
	border-radius:10px;
	background-color:#f8f8f8;
	float:left;
}
#maincontrols td
{
	font-family:arial, sans-serif;
	font-size:12px;
	height:20px;
}
#splaneplot, #zplaneplot
{
	border:0px solid #000;
	padding:0px;
}
#code
{
	padding:5px;
	padding-right:0px;
	overflow:auto;
	width:97%;
	height:400px;
	clear:both;
}
#pageheader
{
	background-image:url(../images/logo.png);
}
@media all and (max-width:910px)
{
	#pageheader, #menu
	{
		background-image:none;
		padding:0px 0px 0px 10px;
	}
	#pageheader h1 .subhead
	{
		margin-left:0px;
		display:block;
		clear:left;
	}
	#topad
	{
		width:100%;
		height:100px;
		margin-left:0px;
		float:none;
	}
	#maintext
	{
		margin:20px 10px;
	}
}
</STYLE>

<body bgcolor="#FFFFFF" link="#0000FF" vlink="#006600" text="#000000">

<DIV id="pageheader">
<H1>Jagged Planet<SPAN class='subhead'>IIR-Explorer</SPAN></H1>
</DIV>
<DIV id="menu">
<UL>
<LI><A href='/'>Home</A></LI> <LI><A href='/vst/jscope.asp'>J-Scope</A></LI> <LI class='selitem'><A href='/iir/iir-explorer.asp'>IIR</A></LI> <LI><A href='/imerge.asp'>iMerge</A></LI> <LI><A href='/terrapin.asp'>Terrapin</A></LI> <LI><A href='/exviewmod.asp'>Astro&nbsp;camera</A></LI> <LI><A href='/raspberrypi.asp'>Raspberry Pi</A></LI> 
</UL>
</DIV>
<DIV id="topad">
<script async src="//pagead2.googlesyndication.com/pagead/js/adsbygoogle.js"></script>
<!-- JaggedPlanet Responsive -->
<ins class="adsbygoogle"
     style="display:block"
     data-ad-client="ca-pub-7472547808115475"
     data-ad-slot="6629101740"
     data-ad-format="auto"></ins>
<script>
(adsbygoogle = window.adsbygoogle || []).push({});
</script>
</DIV>

<div id="maintext">

<H1>IIR Filter Explorer</H1>

<P>
The IIR Filter Explorer is a tool enabling the rapid interactive design of basic Infinite Impulse
Response digital filters. At present, the tool supports Butterworth, Chebyshev (type I and II) and Elliptic filters
up to 20th order, in low-pass, high-pass, band-pass and notch configurations.
</P>
<P>
As well as the visual display, there is an option to audition the effect of the filter on a variety of waveforms.
An example 'C' implementation of the selected filter is output, for copying to your own project. The implementation
can be Nth order for brevity, or a series of biquads for improved stability in more extreme filters.
</P>
<DIV id="maincontrols">
<B>Filter Parameters</B>
 <TABLE>	 <TR><TD height="20"></TD></TR>
 <TR><TD>Type:</TD><TD><SELECT id="filtertype" onchange="javascript:updatectl()" oninput="javascript:updatectl()">
 <OPTION value="b">Butterworth</OPTION>
 <OPTION value="c">Chebyshev</OPTION>
 <OPTION value="i">Chebyshev 2</OPTION>
 <OPTION value="e">Elliptic</OPTION>

 </SELECT></TD></TR>
 <TR><TD>Form:</TD><TD><SELECT id="filtermode" onchange="javascript:updatectl()" oninput="javascript:updatectl()">
 <OPTION value="low">Low-pass</OPTION>
 <OPTION value="high">High-pass</OPTION>
 <OPTION value="band">Band-pass</OPTION>
 <OPTION value="notch">Notch</OPTION>
 </SELECT></TD></TR>
 <TR>
  <TD>Order:</TD>
  <TD><INPUT type="number" id="order" min="1" max="20" value="4" oninput="javascript:updatectl()">
  <INPUT type="range" id="orderslider" min="1" max="20" value="4" onchange="javascript:updatesldr()" oninput="javascript:updatesldr()"></TD>
 </TR>
 <TR>
  <TD>Samplerate:</TD>
  <TD><INPUT type="number" id="samplerate" min="22050" max="88200" step="22050" value="44100" oninput="javascript:updatectl()">
  <INPUT type="range" id="samplerateslider" min="22050" max="88200" step="22050" value="44100" onchange="javascript:updatesldr()" oninput="javascript:updatesldr()"></TD>
 </TR>
 <TR>
  <TD>Cutoff:</TD>
  <TD><INPUT type="number" id="cutoff" min="50" max="22050" step="1" value="10000" oninput="javascript:updatectl()">
  <INPUT type="range" id="cutoffslider" min="50" max="22050" step="1" value="10000" onchange="javascript:updatesldr()" oninput="javascript:updatesldr()"></TD>
 </TR>
 <TR id="ripplerow">
  <TD>Ripple:</TD>
  <TD><INPUT type="number" id="ripple" min="0.0" max="2.999" step="0.001" value="0.1" oninput="javascript:updatectl()">
  <INPUT type="range" id="rippleslider" min="0.0" max="2.999" step="0.001" value="0.1" onchange="javascript:updatesldr()" oninput="javascript:updatesldr()"></TD>
 </TR>
 <TR id="sbripplerow">
  <TD>SB-Ripple:</TD>
  <TD><INPUT type="number" id="sbripple" min="-200.0" max="-3.1" step="0.01" value="-10" oninput="javascript:updatectl()">
  <INPUT type="range" id="sbrippleslider" min="-200.0" max="-3.1" step="0.01" value="-10" onchange="javascript:updatesldr()" oninput="javascript:updatesldr()"></TD>
 </TR>
 <TR id="bandwidthrow">
  <TD>Width:</TD>
  <TD><INPUT type="number" id="bandwidth" min="10" max="22050" step="10" value="100" oninput="javascript:updatectl()">
  <INPUT type="range" id="bandwidthslider" min="10" max="22050" step="10" value="100" onchange="javascript:updatesldr()" oninput="javascript:updatesldr()">
  </TD>
 </TR>
 </TABLE>
</DIV>

<DIV id="freqresponse">
<B>Frequency Response</B>
<BR>
<!--
frq<INPUT type="checkbox" checked="checked" id="plotfreq" onchange="javascript:updatectl()">&nbsp;
-->
frq <SELECT id="plotfreq" onchange="javascript:updatectl()" oninput="javascript:updatectl()" title="Frequency Response">
<OPTION value="none">Hide</OPTION>
<OPTION value="lin">Lin</OPTION>
<OPTION value="log1" selected="selected">Log1</OPTION>
<OPTION value="log10">Log10</OPTION>
</SELECT>
<FONT color="#008000">pha</FONT><INPUT type="checkbox" checked="checked" id="plotphase" title="Phase response" onchange="javascript:updatectl()">&nbsp;
<FONT color="#000080">hpha</FONT><INPUT type="checkbox" id="plothphase" title="Hilbert phase" onchange="javascript:updatectl()">&nbsp;
<FONT color="#800000">grpdly</FONT><INPUT type="checkbox" id="plotgdel" title="Group delay" onchange="javascript:updatectl()">&nbsp;
s-frq<INPUT type="checkbox" id="plotsfreq" title="Unwarped response" onchange="javascript:updatectl()">
<canvas id="responseCanvas" width="300" height="300"
style="background:#000;">
Sorry, this browser does not support canvas
</canvas>
</DIV>

<DIV id="timeresponse">
<B>Time Response</B>
<BR>&nbsp;
Input: <SELECT id="filterinput" onchange="javascript:updatectl()" oninput="javascript:updatectl()">
<OPTION value="square">Square</OPTION>
<OPTION value="saw">Saw</OPTION>
<OPTION value="triangle">Triangle</OPTION>
<OPTION value="sine">Sine</OPTION>
<OPTION value="sum">BL-Saw</OPTION>
<OPTION value="step">Step</OPTION>
<OPTION value="impulse">Impulse</OPTION>
<OPTION value="doublet">Doublet</OPTION>
</SELECT>&nbsp;
octave: <SELECT id="inputoctave" onchange="javascript:updatectl()" oninput="javascript:updatectl()">
<OPTION value="1">1</OPTION>
<OPTION value="2">2</OPTION>
<OPTION value="3">3</OPTION>
<OPTION value="4" selected="selected">4</OPTION>
<OPTION value="5">5</OPTION>
<OPTION value="6">6</OPTION>
<OPTION value="7">7</OPTION>
</SELECT>&nbsp;
show<INPUT type="checkbox" id="showinput" checked="checked" onchange="javascript:updatectl()">&nbsp;
play<INPUT type="checkbox" id="playsound" onchange="javascript:playaudio()">&nbsp;
<canvas id="filtCanvas" width="300" height="300"
style="background:#000;">
Sorry, this browser does not support canvas
</canvas>
</DIV>

<DIV id="plots">
<B>Pole/Zero Plot</B>
<BR>
<INPUT type="radio" name="plottype" id="plottypes" value="s" onchange="javascript:updatectl()">S-Plane&nbsp;&nbsp;
<INPUT type="radio" name="plottype" id="plottypez" value="z" checked="checked" onchange="javascript:updatectl()">Z-Plane&nbsp;&nbsp;
<BR>
<DIV id="splaneplot">
<canvas id="splaneCanvas" width="300" height="300"
style="background:#000;">
Sorry, this browser does not support canvas
</canvas>
</DIV>

<DIV id="zplaneplot">
<canvas id="zplaneCanvas" width="300" height="300"
style="background:#000;">
Sorry, this browser does not support canvas
</canvas>
</DIV>
</DIV>

<DIV id="code">
Code: <SELECT id="codetype" onchange="javascript:updatectl()" oninput="javascript:updatectl()">
 <OPTION value="b">Biquads</OPTION>
 <OPTION value="n">Nth order</OPTION>
</SELECT>
<SPAN id="codebody">
</SPAN>
</DIV>

<script>
<!--
var Complex=function(r,i)
{
	this.r=0;
	this.i=0;
	if (arguments.length==1)
	{
		if (isComplex(r))
		{
			this.r=r.r;
			this.i=r.i;
		}
		else
		{
			this.r=r;
		}
	}
	else if (arguments.length==2)
	{
		this.r=r;
		this.i=i;
	}

	this.clone=function()
	{
		return new Complex(this.r, this.i);
	}
	this.plus=function(c)
	{
		if (isComplex(c))
			return new Complex(this.r+c.r, this.i+c.i);
		else
			return new Complex(this.r+c, this.i);
	}
	this.minus=function(c)
	{
		if (isComplex(c))
			return new Complex(this.r-c.r, this.i-c.i);
		else
			return new Complex(this.r-c, this.i);
	}
	this.multiply=function(c)
	{
		if (isComplex(c))
			return new Complex(this.r*c.r-this.i*c.i, this.r*c.i+this.i*c.r);
		else
			return new Complex(this.r*c, this.i*c);
	}
	this.conjugate=function()
	{
		return new Complex(this.r, -this.i);
	}
	this.negate=function()
	{
		return new Complex(-this.r, -this.i);
	}
	this.divide=function(c)
	{
		if (isComplex(c))
		{
			var m=c.conjugate();
			var den=c.multiply(m).r;
			m=this.multiply(m);
			return new Complex(m.r/den,m.i/den);
		}
		else
			return new Complex(this.r/c, this.i/c);
	}
	this.reciprocal=function()
	{
		return(this.conjugate().divide(this.magnitude2()));
	}
	this.magnitude2=function()
	{
		return this.r*this.r+this.i*this.i;
	}
	this.magnitude=function()
	{
		return Math.sqrt(this.r*this.r+this.i*this.i);
	}
	this.angle=function()
	{
		return Math.atan2(this.i,this.r);
	}
	this.sqrt=function()
	{
		if (this.i==0 && this.r<0)
		{
			return new Complex(0,Math.sqrt(-this.r));
		}
		var r=this.magnitude();
		var zr=this.plus(r);
		var m=Math.sqrt(r)/(zr.magnitude());
		return zr.multiply(m);
	}
	this.cosine=function()
	{
		return new Complex(Math.cos(this.r)*Math.cosh(this.i),-Math.sin(this.r)*Math.sinh(this.i));
	}
}
-->
</script>


<script>
<!--
Math.log10 = Math.log10 || function(x) {
  return Math.log(x) * Math.LOG10E;
};
Math.cosh = Math.cosh || function(x) {
  return (Math.exp(x) + Math.exp(-x)) / 2;
}
Math.acosh = Math.acosh || function(x) {
  return Math.log(x + Math.sqrt(x * x - 1));
};
Math.sinh = Math.sinh || function(x) {
  return (Math.exp(x) - Math.exp(-x)) / 2;
}
Math.asinh = Math.asinh || function(x) {
  if (x === -Infinity) {
    return x;
  } else {
    return Math.log(x + Math.sqrt(x * x + 1));
  }
};
Math.tanh = Math.tanh || function(x){
    var a = Math.exp(+x), b = Math.exp(-x);
    return a == Infinity ? 1 : b == Infinity ? -1 : (a - b) / (a + b);
}
Math.atanh = Math.atanh || function(x) {
  return Math.log((1+x)/(1-x)) / 2;
};

var scnv=document.getElementById("splaneCanvas");
var sctx=scnv.getContext("2d");

var zcnv=document.getElementById("zplaneCanvas");
var zctx=zcnv.getContext("2d");

var rcnv=document.getElementById("responseCanvas");
var rctx=rcnv.getContext("2d");

var ccnv=document.getElementById("filtCanvas");
var cctx=ccnv.getContext("2d");

var audioContext=null;
var filteredWave=null;
var waveGain=null;
window.AudioContext = window.AudioContext||window.webkitAudioContext;
try
{
	audioContext=new AudioContext();
	var bufferSize = 2048,//512,//0.5 * audioContext.sampleRate,
		waveBuffer = audioContext.createBuffer(1, bufferSize, audioContext.sampleRate),
		waveOutput = waveBuffer.getChannelData(0);
	for (var i = 0; i < bufferSize; i++) {
		waveOutput[i] = Math.random() * 2 - 1;
	}
	filteredWave = audioContext.createBufferSource();
	filteredWave.buffer = waveBuffer;
	filteredWave.loop = true;
	waveGain = audioContext.createGain();
	waveGain.gain.value = 0;
	filteredWave.connect(waveGain);
	waveGain.connect(audioContext.destination);
	filteredWave.start(0);
	for (var i = 0; i < bufferSize; i++) {
		waveOutput[i] = 0;
	}
}
catch(e)
{
audioContext=null;
}



var cwid=300;
var chgt=300;

var mouseDown = 0;
var active=true;
var frametime=new Array();
var nframetimes=100;
var winW = 630, winH = 460;

log.msg="";
function log(m)
{

}

function isComplex(n)
{
	return typeof(n)!=typeof(2);
}

// link array items to global vars and UI elements
// for each var, if localstorage exists then init var from localstorage and update UI
// else init var from UI
// when UI changes, update var and call a callback, save var in localstorage
// type can be 'number', 'bool', 'string'

var allUIElements=[]
var UI_Element=function(name,type)
{
	this.name=name;
	this.type=type;
	this.obj=GetObject(name);
	this.value=null;
	allUIElements[allUIElements.length]=this;

	if (typeof(Storage)=="undefined")
	{
		this.Get();
	}
	else
	{
		if (type=="bool")
		{
			this.value=localStorage.getItem(name)=="true"?true:false;
			this.obj.checked=this.value;
		}
		else if (type=="number")
		{
			this.value=parseFloat(localStorage.getItem(name));
			this.obj.value=this.value;
		}
		else 
		{
			this.value=localStorage.getItem(name);
			this.obj.value=this.value;
		}
	}

	this.Store=function()
	{
		localStorage.setItem(this.name,this.value);
	}
	this.Get=function()
	{
		if (this.type=="bool")
			this.value=this.obj.checked?true:false;
		else if (this.type=="number")
			this.value=parseFloat(this.obj.value);
		else this.value=this.obj.value;
		return this.value;
	}

	return this.value;
}

function storeAllUI()
{
	for (i=0; i<allUIElements.length; i++)
		allUIElements[i].Store();
}

function getAllUI()
{
	for (i=0; i<allUIElements.length; i++)
	{
		var o=allUIElements[i];
		var v=o.Get();
		var s=o.name+"="+v;
		alert(s);
		eval(s);	
	}
}


var INF=1E36;
var PI=Math.PI;

var srate=2;
var cutoff=0.2;
var rippledb=0.1;
var sbrippledb=0.1;
var order=3;
var filtertype="b";
var plottype="z";
var codetype="n";

var plotfreq="lin";
var plotfreq=new UI_Element("plotfreq","bool");
var plotphase=false;
var plothphase=false;
var plotgdel=true;
var plotsfreq=false;
var filterinput="square";
var inputoctave=3;
var showinput=false;
var playsound=false;

var poles=[];
var zeros=[];

var zpoles=[];
var zzeros=[];

var npoles=0;
var nzeros=0;
var nzpoles=0;
var nzzeros=0;

var acoeffs=[],bcoeffs=[];
var gain=0;

var biquada=[];
var biquadb=[];

var srate2=srate*2;
var stime=1/srate;
var stime2=0.5*stime;

var smax={r:2*PI, i:2*PI};

function GetObject(name)
{
	var o=null;
	if(document.getElementById)
		o=document.getElementById(name);
	else if(document.all)
		o=document.all.item(name);
	else if(document.layers)
		o=document.layers[name];
	return o;
}

function warp(f)
{
	return 2*Math.tan(PI*f/srate);
}

function chebyshev(order,cutoff,rippledb)
{
	if (rippledb<=0)
		return butterworth(order,cutoff);

	var a=warp(cutoff);

	var e=Math.pow(10,rippledb/20);
	e=Math.sqrt(e*e-1);
	a=a/Math.cosh(Math.acosh(1/e)/order);
	var sx=Math.sinh(Math.asinh(1/e)/order);
	var sy=Math.cosh(Math.asinh(1/e)/order);
	for (var i=0; i<order; i++)
	{
		var j=(i&1)?(order-1-(i>>1)):(i>>1); // generate in conjugate pair order
		var th=(2*j+order+1)*PI/(2*order);
		poles[i]=new Complex(Math.cos(th)*a*sx,Math.sin(th)*a*sy);
	}
	npoles=order;
	nzeros=0;
}

function chebyshev2(order,cutoff,rippledb)
{
	var a=warp(cutoff);

	var e=Math.pow(10,-rippledb/10);
	e=1/Math.sqrt(e-1);
	a=a*Math.cosh(Math.acosh(1/e)/order);
	var sx=Math.sinh(Math.asinh(1/e)/order);
	var sy=Math.cosh(Math.asinh(1/e)/order);
	for (var i=0; i<order; i++)
	{
		var j=(i&1)?(order-1-(i>>1)):(i>>1); // generate in conjugate pair order
		var th=(2*j+order+1)*PI/(2*order);
		var p=new Complex(Math.cos(th)*sx,Math.sin(th)*sy);
		poles[i]=new Complex(a,0).divide(p);
		var z=new Complex(0,-Math.sin(th));
		zeros[i]=new Complex(a,0).divide(z);
	}
	npoles=order;
	nzeros=order;
}

function butterworth(order,cutoff)
{
	var a=warp(cutoff);

	for (var i=0; i<order; i++)
	{
		var j=(i&1)?(order-1-(i>>1)):(i>>1); // generate in conjugate pair order
		var th=(2*j+order+1)*PI/(2*order);
		poles[i]=new Complex(Math.cos(th)*a,Math.sin(th)*a);
		if (poles[i].i<1E-10 && poles[i].i>-1e-10) poles[i].i=0;
	}
	npoles=order;
	nzeros=0;
}

// This function is adapted from the C code provided by Iowa Hills software
// http://iowahills.com/A7ExampleCodePage.html
// This code was described in "Elliptic Functions for Filter Design"
// H J Orchard and Alan N Willson  IEE Trans on Circuits and Systems April 97
// The equation numbers in the comments are from the paper.
// As the stop band attenuation -> infinity, the Elliptic -> Chebyshev.
function elliptic(order, cutoff, rippledb, sbrippledb)
{
	var j, k, n, LastK;
	var MAX_ELLIP_ITER = 15;
	var ELLIPARRAYSIZE=(order<10)?11:(order+1);  // needs to be > 10 and >= Max Num Poles + 1

	var K=[], G=[], Epsilon=[];
	var A, D, SBdB, dBErr, RealPart, ImagPart;
	var DeltaK, PrevErr, Deriv;
	var C=new Complex(0,0);

	if (sbrippledb<=-200)
	{
		chebyshev(order,cutoff,rippledb);
		return;
	}
	if (rippledb<=0.0)
	{
		chebyshev2(order,cutoff,sbrippledb);
		return;
	}
/*
*/
	var a=warp(cutoff);

	for(j=0; j<ELLIPARRAYSIZE; j++)
	{
		K[j] = 0.0;
		G[j] = 0.0;
		Epsilon[j] = 0.0;
	}
//	if(rippledb < 0.001)
//		rippledb = 0.001;
//	if(rippledb > 1.0)
//		rippledb = 1.0;
	Epsilon[0] = Math.sqrt(Math.pow(10.0, rippledb/10.0) - 1.0);

	// Estimate K[0] to get the algorithm started.
	K[0] = (order-2)*0.1605 + 0.016;
	if(K[0] < 0.01)
		K[0] = 0.01;
	if(K[0] > 0.7)
		K[0] = 0.7;

	// This loop calculates K[0] for the desired stopband attenuation. It typically loops < 5 times.
	for(j=0; j<MAX_ELLIP_ITER; j++)
	{
		// Compute K with a forward Landen Transformation.
		for(k=1; k<10; k++)
		{
			K[k] = Math.pow( K[k-1] / (1.0 + Math.sqrt(1.0 - K[k-1]*K[k-1]) ), 2.0);   // eq. 10
			if(K[k] <= 1.0E-6)
				break;
		}
		LastK = k;

		// Compute G with a backwards Landen Transformation.
		G[LastK] = 4.0 * Math.pow( K[LastK] / 4.0, order);
		for(k=LastK; k>=1; k--)
		{
			G[k-1] = 2.0 * Math.sqrt(G[k]) / (1.0 + G[k]) ;  // eq. 9
		}

		if(G[0] <= 0.0)
			G[0] = 1.0E-10;
		SBdB = 10.0 * Math.log10(1.0 + Math.pow(Epsilon[0]/G[0], 2.0)); // Current stopband attenuation dB
		dBErr = -sbrippledb - SBdB;

		if(Math.abs(dBErr) < 0.1)
			break;
		if(j==0) // Do this on the 1st loop so we can calc a derivative.
		{
			if(dBErr > 0)
				DeltaK = 0.005;
			else 
				DeltaK = -0.005;
			PrevErr = dBErr;
		}
		else
		{
			// Use Newtons Method to adjust K[0].
			Deriv = (PrevErr - dBErr)/DeltaK;
			PrevErr = dBErr;
			if(Deriv == 0.0)
				break; // This happens when K[0] hits one of the limits set below.
			DeltaK = dBErr/Deriv;
			if(DeltaK > 0.1)
				DeltaK = 0.1;
			if(DeltaK < -0.1)
				DeltaK = -0.1;
		}
		K[0] -= DeltaK;
		if(K[0] < 0.001)
			K[0] = 0.001;  // must not be < 0.0
		if(K[0] > 0.990)
			K[0] = 0.990;  // if > 0.990 we get a pole in the RHP. This means we were unable to set the stop band atten to the desired level (the Ripple is too large for the Pole Count).
	}


	// Epsilon[0] was calulated above, now calculate Epsilon[LastK] from G
	for(j=1; j<=LastK; j++)
	{
		A = (1.0 + G[j]) * Epsilon[j-1] / 2.0;  // eq. 37
		Epsilon[j] = A + Math.sqrt(A*A + G[j]);
	}

	// Calulate the poles and zeros.
	ImagPart = Math.log((1.0 + Math.sqrt(1.0 + Epsilon[LastK]*Epsilon[LastK])) / Epsilon[LastK] ) / order;  // eq. 22
	n = 0;
	for(j=1; j<=order/2; j++)
	{
		RealPart = (2*j - 1) * Math.PI*0.5 / order;   // eq. 19
		C = new Complex(0.0, -1.0).divide(new Complex(-RealPart, ImagPart).cosine());      // eq. 20
		D = 1.0 / Math.cos(RealPart);
		for(k=LastK; k>=1; k--)
		{
			C = (C.minus(C.reciprocal().multiply(K[k]))).divide(1.0 + K[k]);  // eq. 36
			D = (D + K[k]/D) / (1.0 + K[k]);
		}

		poles[n] = C.reciprocal();//.multiply(a);
		poles[n+1] = poles[n].conjugate();
		zeros[n] = new Complex(0.0, D/K[0]);//.multiply(a);
		zeros[n+1] = zeros[n].conjugate();
		n+=2;
	}
	nzeros = n; // n is the num zeros

	if(order&1)   // The real pole for odd pole counts
	{
		A = 1.0 / Math.sinh(ImagPart);
		for(k=LastK; k>=1; k--)
		{
			A = (A - K[k]/A) / (1.0 + K[k]);      // eq. 38
		}
		poles[n] = new Complex(-1.0/A, 0.0);//.multiply(a);
		n++;
	}
	npoles = n;

	//adjust poles,zeros so that 3dB point is at cutoff
	function resp(f)
	{
		var a=(2*PI*f/srate);

		var w=new Complex(0,a);
		var num=new Complex(1,0);
		var den=new Complex(1,0);
		for (var i=0; i<npoles; i++)
		{
			den=den.multiply(w.minus(poles[i]));
		}
		for (var i=0; i<nzeros; i++)
		{
			num=num.multiply(w.minus(zeros[i]));
		}
		return num.divide(den);
	}
	var r=[];
	var nr=100;
	var pk2=0;
	for (var f=0;f<nr;f++)
	{
		r[f]=resp(f/nr*srate*0.5).magnitude2();
		if (r[f]>pk2) pk2=r[f];
	}
	//find locations where r[f]=pk2*0.5 and r[f]=pk2-rippledb
	var pk=pk2*0.5;
	var pr=pk2*Math.pow(10,-rippledb/10)
	var fc=-1,fr=-1;
	for (f=1;f<nr;f++)
	{
		if (r[f-1]>=pk && r[f]<pk)
		{
			fc=f-1+((pk-r[f-1])/(r[f]-r[f-1]));
		}
		if (r[f-1]>=pr && r[f]<pr)
		{
			fr=f-1+((pr-r[f-1])/(r[f]-r[f-1]));
		}
	}
	fc=fc/nr*srate*0.5;
	fr=fr/nr*srate*0.5;
	var ac=a*fr/fc;//warp(fc);
	for (var i=0;i<npoles;i++)
		poles[i]=poles[i].multiply(ac);
	for (var i=0;i<nzeros;i++)
		zeros[i]=zeros[i].multiply(ac);
}



function response(ctx)
{
	function resp(f)
	{
		var a=(2*PI*f/srate);

		var w=new Complex(0,a);
		var num=new Complex(1,0);
		var den=new Complex(1,0);
		for (var i=0; i<npoles; i++)
		{
			den=den.multiply(w.minus(poles[i]));
		}
		for (var i=0; i<nzeros; i++)
		{
			num=num.multiply(w.minus(zeros[i]));
		}
		return num.divide(den);
	}
	function zresp(f)
	{
		var a=f*PI;
		var mag=1;
		var w=new Complex(Math.cos(a),Math.sin(a));
		var num=new Complex(1,0);
		var den=new Complex(1,0);
		for (var i=0; i<nzpoles; i++)
		{
			den=den.multiply(w.minus(zpoles[i]));
		}
		for (var i=0; i<nzzeros; i++)
		{
			num=num.multiply(w.minus(zzeros[i]));
		}
		return num.divide(den);
	}

	var x0=10;
	var x1=cwid-10;
	var xs=x1-x0;
	var y0=10;
	var y1=chgt-10;
	var ys=y1-y0;

	ctx.beginPath();
	ctx.strokeStyle="#000000";
	ctx.strokeWidth=1;
	ctx.moveTo(x0,chgt-y1);
	ctx.lineTo(x0,chgt-y0);
	ctx.lineTo(x1,chgt-y0);
	ctx.stroke();
	for (i=1; i<=10; i++)
	{
		ctx.strokeStyle="#E0E0E0";
		ctx.beginPath();
		ctx.moveTo(x0+i*xs/10,chgt-y0);
		ctx.lineTo(x0+i*xs/10,chgt-y1);
		ctx.stroke();

		ctx.strokeStyle="#000000";
		ctx.beginPath();
		ctx.moveTo(x0+i*xs/10,chgt-5);
		ctx.lineTo(x0+i*xs/10,chgt-15);
		ctx.stroke();

		var h=i*ys/10;
		ctx.strokeStyle="#E0E0E0";
		ctx.beginPath();
		ctx.moveTo(x0,chgt-y0-h);
		ctx.lineTo(x1,chgt-y0-h);
		ctx.stroke();

		ctx.strokeStyle="#000000";
		ctx.beginPath();
		ctx.moveTo(x0-5,chgt-y0-h);
		ctx.lineTo(x0+5,chgt-y0-h);
		ctx.stroke();
	}
	if (plotfreq!="none")
	{
		var h3db=ys/Math.sqrt(2);
		if (plotfreq=="log1")
			h3db=ys*(1+2*Math.log10(1/Math.sqrt(2)));
		else if (plotfreq=="log10")
			h3db=ys*(1+0.2*Math.log10(1/Math.sqrt(2)));
		ctx.strokeStyle="#F08080";
		ctx.beginPath();
		ctx.moveTo(x0,chgt-y0-h3db);
		ctx.lineTo(x1,chgt-y0-h3db);
		ctx.stroke();
	}
/*
Plot results of z-transform, should be same as de-warped s-transform
If asked to show s too, show warped s as dotted lines
*/

	for (pass=0; pass<(plotsfreq?2:1); pass++)
	{
		// pass 0=normal z-transform
		// pass 1=warped s-transform if requested

		var r=[];
		var g=0
		var peak=0;
		var centre=Math.round(cutoff/srate*2*xs);
		for (var x=x0; x<x1; x++)
		{
			var f=(x-x0)/xs;
			if (pass==1)
			{
				f=srate*0.5*f;
				r[x-x0]=resp(f);
			}
			else
			{
				r[x-x0]=zresp(f);
			}
			r[x-x0].mag=r[x-x0].magnitude();
			r[x-x0].ang=r[x-x0].angle();
			var y=r[x-x0].mag;
			if (y>g) 
			{
				g=y;
				peak=x-x0;
			}
		}
		if (pass==0)
		{
			gain=g;
		}
		var rg=1/g;
		if (filtermode=="low") peak=0; else if (filtermode!="band") peak=r.length-1;
		// sort out phase wrapping
		{
			var ph=r[0].ang;
			var dph=0;
			for (var x=1; x<r.length; x++)
			{
				r[x].ang+=dph;
				if (r[x].ang-ph>PI)
				{
					dph-=2*PI;
					r[x].ang-=2*PI;
				}
				else if (r[x].ang-ph<-PI)
				{
					dph+=2*PI;
					r[x].ang+=2*PI;
				}
				ph=r[x].ang;
			}
			ph=r[peak].ang;
			for (var x=0; x<r.length; x++)
				r[x].ang-=ph;
		}

		var minmag=0.15;
		if (pass==1)
			ctx.setLineDash([2]);
		if (plotfreq!="none")
		{
			ctx.strokeStyle="#000000";
			ctx.beginPath();
			ctx.moveTo(x0,chgt-y0);
			var r0=r[0];
			if (plotfreq!="lin")
			{
				var prevy=y1;
				var dv=(plotfreq=="log1")?1:10;
				for (var x=x0; x<x1; x++)
				{
					var y=rg*r[x-x0].mag;
					try
					{
						y=1+2*Math.log10(y)/dv;
					}
					catch(e)
					{
						y=0;
					}
					y=y*ys+y0;
					if (y<y0) y=y0;
					if (prevy<=y0)
						ctx.moveTo(x,chgt-y0);
					else
						ctx.lineTo(x,chgt-y);
					prevy=y;
				}
			}
			else
			{
				for (var x=x0; x<x1; x++)
				{
					var y=rg*r[x-x0].mag*ys+y0;
					ctx.lineTo(x,chgt-y);
				}
			}
			ctx.stroke();
		}
		if (plotphase)
		{
			ctx.strokeStyle="#008000";
			ctx.beginPath();
			var ph=(r[0].angle()+PI)/(2*PI)*ys+y0
			ctx.moveTo(x0,chgt-ph);
			for (var x=x0; x<x1; x++)
			{
				var y=(r[x-x0].angle()+PI)/(2*PI)*ys+y0;
				if (y-ph>chgt*0.4 || ph-y>chgt*0.4 || r[x-x0].mag*rg<minmag)
					ctx.moveTo(x,chgt-y);
				else
					ctx.lineTo(x,chgt-y);
				ph=y;
			}
			ctx.stroke();
		}


		if (plothphase)
		{
			ctx.strokeStyle="#000080";
			ctx.beginPath();
			var pk=0;
			if (filtermode=="high") pk=r.length-1;
			if (filtermode=="band") pk=centre;

			var ph=(r[pk].ang+PI)/(2*PI)*ys;
			var dpdx;
			if (pk==r.length-1)
				dpdx=ph-((r[pk-1].ang+PI)/(2*PI)*ys);
			else
				dpdx=((r[pk+1].ang+PI)/(2*PI)*ys)-ph;
			var ph=(r[0].ang+PI)/(2*PI)*ys;
			var yc=chgt*0.5;
			phoff=pk*dpdx;
			var y=(r[0].ang+PI)/(2*PI)*ys+y0;
			while (y<y0) {y+=ys; phoff+=ys;}
			while (y>=y1) {y-=ys; phoff-=ys;}
			for (var x=x0; x<x1; x++)
			{
				if (filtermode=="notch" && x-x0==centre)
				{
					pk=r.length-1;
					ph=(r[pk].ang+PI)/(2*PI)*ys;
					dpdx=ph-((r[pk-1].ang+PI)/(2*PI)*ys);
					ph=(r[x-x0].ang+PI)/(2*PI)*ys;
					phoff=(pk)*dpdx;
				}
				var y=(r[x-x0].ang+PI)/(2*PI)*ys+y0;
				y-=dpdx*(x-x0);
				y+=phoff;
				while (y<y0) y+=ys;
				while (y>=y1) y-=ys;
				if (y-ph>chgt*0.4 || ph-y>chgt*0.4 || r[x-x0].mag*rg<minmag)
					ctx.moveTo(x,chgt-y);
				else
				{
					if (x==x0)
						ctx.moveTo(x,chgt-y);
					else
						ctx.lineTo(x,chgt-y);
				}
				ph=y;
			}
			ctx.stroke();
		}	
		if (plotgdel)
		{
			ctx.strokeStyle="#800000";
			ctx.beginPath();
			ctx.moveTo(x0,chgt-ph);
			var rdw=(x1-x0)/PI;
			var mx=0;
			for (var x=x0+1; x<x1-1; x++)
			{
				var y=(r[x-x0].ang-r[x+1-x0].ang)/(2*PI)*rdw;
				if (y>mx) mx=y;
			}
			mx=0.4/mx;
			for (var x=x0+1; x<x1-1; x++)
			{
				var y=(r[x-x0].ang-r[x+1-x0].ang)/(2*PI)*ys*rdw*mx+y0;
				if (x==x0+1 || r[x-x0].mag*rg<minmag)
					ctx.moveTo(x,chgt-y);
				else
					ctx.lineTo(x,chgt-y);
			}
			ctx.stroke();
		}
	}
	ctx.setLineDash([1,0]);
}

function drawpole(ctx,x,y,n)
{
	if (x<0) x=0; if (x>cwid) x=cwid;
	if (y<0) y=0; if (y>chgt) y=chgt;
	ctx.fillStyle="#FF0000";
	ctx.beginPath();
	ctx.arc(x,y,3,0,2*Math.PI);
//	ctx.strokeText(n,x+5,y);
	ctx.fill();
}
function drawzero(ctx,x,y,n)
{
	if (x<0) x=0; if (x>cwid) x=cwid;
	if (y<0) y=0; if (y>chgt) y=chgt;
	ctx.strokeStyle="#000000";
	ctx.beginPath();
	ctx.arc(x,y,3,0,2*Math.PI);
//	ctx.strokeText(n,x+5,y);
	ctx.stroke();
}

function drawsplane()
{
	var cx=Math.floor(cwid*0.5)+.5;
	var cy=Math.floor(chgt*0.5)+.5;
	sctx.fillStyle="#FFFFFF";
	sctx.fillRect(cx,0,cwid,chgt);
	sctx.fillStyle="#E0E0E0";
	sctx.fillRect(0,0,cx,chgt);
	sctx.strokeStyle="#000000";
	sctx.strokeWidth=1;
	sctx.beginPath();
	sctx.moveTo(0,cy);
	sctx.lineTo(cwid,cy);
	sctx.moveTo(cx,0);
	sctx.lineTo(cx,chgt);
	sctx.stroke();
	var sx=cx/smax.r;
	var sy=cy/smax.i;
	for (var i=0;i<npoles;i++)
	{
		var p=poles[i];
		drawpole(sctx,cx+sx*p.r, cy+sy*p.i, i);
	}
	for (var i=0;i<nzeros;i++)
	{
		var z=zeros[i];
		drawzero(sctx,cx+sx*z.r, cy+sy*z.i, i);
	}
	log("s-poles: ");
	for (var i=0;i<npoles;i++)
	{
		log("("+poles[i].r.toPrecision(5)+", "+poles[i].i.toPrecision(5)+") ");
	}
	if (nzeros)
	{
		log("<BR>s-zeros: ");
		for (var i=0;i<nzeros;i++)
		{
			log("("+zeros[i].r.toPrecision(5)+", "+zeros[i].i.toPrecision(5)+") ");
		}
	}
}

function drawzplane()
{
	function tform(s)
	{
		var t=s.plus(2).divide(s.minus(2).negate());
		return t;
	}

	var cx=Math.floor(cwid*0.5)+.5;
	var cy=Math.floor(chgt*0.5)+.5;
	zctx.fillStyle="#FFFFFF";
	zctx.fillRect(0,0,cwid,chgt);
	zctx.fillStyle="#E0E0E0";
	zctx.strokeStyle="#000000";
	zctx.strokeWidth=1;
	zctx.beginPath();
	zctx.arc(cx,cy,cx*0.75,0,2*Math.PI);
	zctx.fill();
	zctx.stroke();
	zctx.beginPath();
	zctx.moveTo(0,cy);
	zctx.lineTo(cwid,cy);
	zctx.moveTo(cx,0);
	zctx.lineTo(cx,chgt);
	zctx.stroke();
	var sx=cx*0.75;
	var sy=cy*0.75;

	// transform s-plane poles and zeros to z-plane
	for (var i=0; i<npoles; i++)
	{
		zpoles[i]=tform(poles[i]);
	}
	nzpoles=npoles;
	for (var i=0; i<nzeros; i++)
	{
		zzeros[i]=tform(zeros[i]);
	}
	nzzeros=nzeros;
	for (var i=0; i<npoles-nzeros; i++)
	{
		zzeros[nzzeros++]=new Complex(-1,0);
	}
	for (var i=0; i<nzeros-npoles; i++)
	{
		zpoles[nzpoles++]=new Complex(-1,0);
	}

	log("<BR>z-poles: ");
	for (var i=0;i<nzpoles;i++)
	{
		log("("+zpoles[i].r.toPrecision(5)+", "+zpoles[i].i.toPrecision(5)+") ");
	}
	log("<BR>z-zeros: ");
	for (var i=0;i<nzzeros;i++)
	{
		log("("+zzeros[i].r.toPrecision(5)+", "+zzeros[i].i.toPrecision(5)+") ");
	}

	for (var i=0;i<nzpoles;i++)
	{
		var pol=zpoles[i];
		var x=pol.r;
		var y=pol.i;

		drawpole(zctx,cx+sx*x, cy+sy*y, i);
	}
	for (var i=0;i<nzzeros;i++)
	{
		var zer=zzeros[i];
		var x=zer.r;
		var y=zer.i;

		drawzero(zctx,cx+sx*x, cy+sy*y, i);
	}
}


function clear()
{
	sctx.fillStyle="#FFFFFF";
	sctx.fillRect(0,0,cwid,chgt);
	zctx.fillStyle="#FFFFFF";
	zctx.fillRect(0,0,cwid,chgt);
	rctx.fillStyle="#FFFFFF";
	rctx.fillRect(0,0,cwid,chgt);
	cctx.fillStyle="#FFFFFF";
	cctx.fillRect(0,0,cwid,chgt);
}

function saveparams()
{
	if (typeof(Storage)=="undefined")
		return;
	localStorage.srate=srate;
	localStorage.cutoff=cutoff;
	localStorage.sbrippledb=sbrippledb;
	localStorage.rippledb=rippledb;
	localStorage.order=order;
	localStorage.codetype=codetype;
	localStorage.plottype=plottype;
	localStorage.filtertype=filtertype;
	localStorage.filtermode=filtermode;
	localStorage.bandwidth=bandwidth;
	localStorage.plotfreq=plotfreq;
	localStorage.plotphase=plotphase;
	localStorage.plothphase=plothphase;
	localStorage.plotgdel=plotgdel;
	localStorage.plotsfreq=plotsfreq;
	localStorage.filterinput=filterinput;
	localStorage.inputoctave=inputoctave;
	localStorage.showinput=showinput;
	localStorage.playsound=playsound;
//storeAllUI()
}

function restoreparams()
{
	if (typeof(Storage)=="undefined" || typeof(localStorage.srate)=="undefined")
		return;
	srate=localStorage.srate;
	cutoff=localStorage.cutoff;
	rippledb=localStorage.rippledb;
	sbrippledb=localStorage.sbrippledb;
	order=localStorage.order;
	plottype=localStorage.plottype;
	codetype=localStorage.codetype;
	filtertype=localStorage.filtertype;
	filtermode=localStorage.filtermode;
	bandwidth=localStorage.bandwidth;
	plotfreq=localStorage.plotfreq;
	plotphase=localStorage.plotphase=="true";
	plothphase=localStorage.plothphase=="true";
	plotgdel=localStorage.plotgdel=="true";
	plotsfreq=localStorage.plotsfreq=="true";
	filterinput=localStorage.filterinput;
	inputoctave=localStorage.inputoctave;
	showinput=localStorage.showinput=="true";
	playsound=localStorage.playsound=="true";
	GetObject("samplerate").value=srate;
	GetObject("cutoff").value=cutoff;
	GetObject("ripple").value=rippledb;
	GetObject("sbripple").value=sbrippledb;
	GetObject("order").value=order;
	GetObject("plottypez").checked=(plottype=="z");
	GetObject("plottypes").checked=(plottype=="s");
	GetObject("codetype").value=codetype;
	GetObject("filtertype").value=filtertype;
	GetObject("filtermode").value=filtermode;
	GetObject("bandwidth").value=bandwidth;
	GetObject("plotfreq").value=plotfreq;
	GetObject("plotphase").checked=plotphase?"checked":"";
	GetObject("plothphase").checked=plothphase?"checked":"";
	GetObject("plotgdel").checked=plotgdel?"checked":"";
	GetObject("plotsfreq").checked=plotsfreq?"checked":"";
	GetObject("filterinput").value=filterinput;
	GetObject("inputoctave").value=inputoctave;
	GetObject("showinput").checked=showinput?"checked":"";
	GetObject("playsound").checked=playsound?"checked":"";
//getAllUI();
}

function makehighpass()
{
	var a=warp(cutoff);
	var r=new Complex(a*a,0);
	for (var i=0; i<nzeros; i++)
	{
		zeros[i]=r.divide(zeros[i]);
	}
	for (var i=0; i<npoles; i++)
	{
		poles[i]=r.divide(poles[i]);
	}
	for (var i=nzeros; i<npoles; i++)
	{
		zeros[nzeros++]=new Complex(0,0);
	}
}

function makebandpass()
{
/*
s - - > (s^2 + w0^2)/(s(wd))
=(s-iw0)(s+iw0)/(swd)   w0=cutoff=sqrt(w1.w2)  wd=w2-w1

(s-p) - - > (s^2 - sp(wd) + w0^2)/swd)
pole = (p.wd +/- sqrt(p.p.wd.wd - 4.w0^2))/2
zero=0
pole from p moves to p+/-iw0

*/

	var bw=bandwidth;
	if (bw>cutoff) bw=cutoff;
	if (bw>srate/2-cutoff) bw=srate/2-cutoff;
	var w0=warp(cutoff);

	var w2=warp(cutoff+bw/2);
	var w1=warp(cutoff-bw/2);
	w0=Math.sqrt(w2*w1);
	var wd=w2-w1;
	
	//zeros and poles are in conjugate pairs, so ensure new ones preserve that pattern

	var np=npoles;
	for (var i=0; i<np; i++)
	{
		var pwd=poles[i].multiply(wd/w0);
		var d=pwd.multiply(pwd).minus(new Complex(4*w0*w0));
		d=d.sqrt();
		poles[i]=pwd.plus(d).multiply(0.5);
		poles[np*2-1-i]=pwd.minus(d).multiply(0.5);
		npoles++;
	}
	var nz=nzeros;
	for (var i=0; i<nz; i++)
	{
		var pwd=zeros[i].multiply(wd/w0);
		var d=pwd.multiply(pwd).minus(new Complex(4*w0*w0));
		d=d.sqrt();
		if((i&1)==0)
		{
			zeros[nz*2-1-i]=pwd.plus(d).multiply(0.5);
			zeros[i]=pwd.minus(d).multiply(0.5);
		}
		else
		{
			zeros[i]=pwd.plus(d).multiply(0.5);
			zeros[nz*2-1-i]=pwd.minus(d).multiply(0.5);
		}
		nzeros++;
	}

	for (var i=nzeros/2; i<npoles/2; i++)
		zeros[nzeros++]=new Complex(0,0);
}

function makenotch()
{
/*
s - - > s(wd)/(s^2 + w0^2)
s-p - - > (-p.s^2 + s.wd - p.w0^2)/(s^2 + w0^2)

pole = (wd +/- sqrt(wd^2 - 4.p^2.w0^2))/2p
=(wd/p +/- sqrt((wd/p)^2 - w.w0^2))/2
zero=+/-iw0
*/

	var bw=bandwidth;
	if (bw>cutoff) bw=cutoff;
	if (bw>srate/2-cutoff) bw=srate/2-cutoff;
	var w0=warp(cutoff);

	var w2=warp(cutoff+bw/2);
	var w1=warp(cutoff-bw/2);
	w0=Math.sqrt(w2*w1);
	var wd=w2-w1;
	
	//zeros and poles are in conjugate pairs, so ensure new ones preserve that pattern

	var np=npoles;
	for (var i=0; i<np; i++)
	{
		var pwd=new Complex(wd*w0).divide(poles[i]);
		var d=pwd.multiply(pwd).minus(new Complex(4*w0*w0));
		d=d.sqrt();
		{
			poles[i]=pwd.plus(d).multiply(0.5);
			poles[np*2-1-i]=pwd.minus(d).multiply(0.5);
		}
		npoles++;
	}
	var nz=nzeros;
	for (var i=0; i<nz; i++)
	{
		var pwd=new Complex(wd*w0).divide(zeros[i]);
		var d=pwd.multiply(pwd).minus(new Complex(4*w0*w0));
		d=d.sqrt();
		if(i&1)
		{
			zeros[nz*2-1-i]=pwd.plus(d).multiply(0.5);
			zeros[i]=pwd.minus(d).multiply(0.5);
		}
		else
		{
			zeros[i]=pwd.plus(d).multiply(0.5);
			zeros[nz*2-1-i]=pwd.minus(d).multiply(0.5);
		}
		nzeros++;
	}

	for (var i=nzeros/2; i<npoles/2; i++)
	{
		zeros[nzeros++]=new Complex(0,w0);
		zeros[nzeros++]=new Complex(0,-w0);
	}
}

function filter(ctx)
{
	var yv=[],xv=[];
	var xyv=[];
	if (codetype=="n")
	{
		for (var i=0; i<nzpoles+1; i++)
			yv[i]=0;
		for (var i=0; i<nzzeros+1; i++)
			xv[i]=0;
	}
	else
	{
		for (var i=0;i<3+3*((nzpoles+1)>>1); i++)
			xyv[i]=0;
	}

	function fstep(v)
	{
		if (codetype=="n")
		{
			for (var i=0; i<nzzeros; i++)
				xv[i]=xv[i+1];
			xv[nzzeros] = v/gain;
			for (var i=0; i<nzpoles; i++)
				yv[i]=yv[i+1];
			var op=0;
			for (var i=0; i<=nzzeros; i++)
				op+=xv[i]*bcoeffs[i];
			for (var i=0; i<nzpoles; i++)
				op-=yv[i]*acoeffs[i];
			
			yv[nzpoles]=op;
			return op;
		}
		else
		{
			// series of biquads
			var op=v/gain;

			var xp=0,yp=nzzeros>1?3:2;
			var bqp=0;
			for (var i=xyv.length-1; i>0; i-=1)
			{
				xyv[i]=xyv[i-1];
			}
			var mx=((nzzeros+1)>>1);
			for (var b=0; b<mx; b++)
			{
				var len=((b==mx-1)&&(nzzeros&1))?1:2;
				xyv[xp] = op;	

				for (var i=0; i<len; i++)
					op+=(xyv[xp+len-i]*biquadb[bqp+i]-xyv[yp+len-i]*biquada[bqp+i]);
				bqp+=len;
				xyv[yp]=op;
				xp=yp;
				yp+=len+1;
			}
			return op;
		}
	}
	var shft=10-inputoctave;
	var shftd=(1<<shft);
	var msk=(1<<shft)-1;
	var hlf=(1<<(shft-1));
	var mlt=1/msk;
	function finput(x)
	{
		switch(filterinput)
		{
			case "square":
				return (((x>>(shft-1))&1)?1:-1)*0.2;
			case "saw":
				return (((x&msk)-hlf))*mlt*0.5;
			case "triangle":
				return (Math.abs((x&msk)-hlf)-hlf*0.5)*mlt;
			case "sine":
				return Math.sin((x&msk)*2*PI/shftd)*0.3;
			case "sum":
				{
					var v=0;
					var dv=1;
					var f=(x&msk)*2*PI/shftd
					for (var i=1; i<71-inputoctave; i++)
					{
						v+=Math.sin(f*i)*0.1/i;
					}
					return v;
				}
			case "step":
				return (x<x0+2)?-0.3:0.3;
			case "impulse":
				return (x==x0+2)?1:0;
			case "doublet":
				return (x==x0+2)?1:(x==x0+3?-1:0);
		}
	}

	var x0=10;
	var x1=cwid-10;
	var xs=x1-x0;
	var y0=10;
	var y1=chgt-10;
	var ys=y1-y0;
	var yc=(y0+y1)/2;

	ctx.strokeStyle="#000000";
	ctx.strokeWidth=1;
	ctx.beginPath();
	var y;
	for (var x=x0-5000; x<x0; x++)
	{
		y=fstep(finput(x));
	}
	ctx.moveTo(x0-1,yc-y*ys);
	for (var x=x0; x<x1; x++)
	{
		y=fstep(finput(x));
		ctx.lineTo(x, yc-y*ys)
	}
	ctx.stroke();

	if (audioContext!=null)
	{
		for (var i=0;i<bufferSize; i++)
		{
			var o=fstep(finput((x+i)&(bufferSize-1)));
			if (o<-0.5) 
				o=-0.5; 
			else if (o>0.5) 
				o=0.5;
			waveOutput[i]=o;
		}
	}
	
	if (showinput)
	{
		ctx.setLineDash([2]);
		ctx.strokeStyle="#f000f0";
		ctx.beginPath();
		var y=finput(x0-1);
		ctx.moveTo(x0-1,yc-y*ys);
		for (var x=x0; x<x1; x++)
		{
			y=finput(x);
			ctx.lineTo(x, yc-y*ys)
		}
		ctx.stroke();
		ctx.setLineDash([0]);
	}
}


/*
poles and zeros are either real or in conjugate pairs
so product of all n (z-pi) is a real polynomial with terms from z^0 up to z^n
similarly product of all (z-zi)
multiply each by z^-n to get polynomial in z^-1
divide by coefficient of z^0 to make constant term = +1
*/
function getcoeffs()
{
	var cp=[],rp=[],cz=[],rz=[];
	var eps=1.0e-10;

	if (codetype=="n")
	{
		var a=[new Complex(1,0)];
		var b=[new Complex(1,0)];

		for (var i=0; i<nzpoles; i++)
		{
			var p=zpoles[i];
			// multiply polynomial in a by z-p
			var len=a.length;
			a[len]=new Complex(0,0);
			for (var j=len; j>0; j-=1)
			{
				a[j]=a[j-1].minus(a[j].multiply(p));
			}
			a[0]=a[0].multiply(p.negate());
		}
		acoeffs=[];
		//should all be real numbers
		for (var i=0;i<a.length;i++)
			acoeffs[i]=a[i].r;

		for (var i=0; i<nzzeros; i++)
		{
			var p=zzeros[i];
			// multiply polynomial in b by z-p
			var len=b.length;
			b[len]=new Complex(0,0);
			for (var j=len; j>0; j-=1)
			{
				b[j]=b[j-1].minus(b[j].multiply(p));
			}
			b[0]=b[0].multiply(p.negate());
		}
		bcoeffs=[];
		//should all be real numbers
		for (var i=0;i<b.length;i++)
			bcoeffs[i]=b[i].r;


		log("<BR>Gain: "+gain.toPrecision(5));
		log("<BR>A values: ");
		for (var i=0;i<acoeffs.length;i++)
		{
			log(acoeffs[i].toPrecision(5)+" ");
		}
		log("<BR>B values: ");
		for (var i=0;i<bcoeffs.length;i++)
		{
			log(bcoeffs[i].toPrecision(5)+" ");
		}
	}
	else
	{ //codetype=="b"
		// also generate coefficients decomposed into a list of biquads
		biquada=[];
		biquadb=[];

		for (var i=0; i<nzpoles-1; i+=2)
		{
			var a=[new Complex(1,0)];
			var b=[new Complex(1,0)];

			//zpoles[i] and [i+1] should be conjugate
			a[1]=zpoles[i].plus(zpoles[i+1]).negate();
			a[0]=zpoles[i].multiply(zpoles[i+1]);
	
			//should all be real numbers
			for (var j=0;j<a.length;j++)
				biquada[biquada.length]=a[j].r;

			//zzeros[i] and [i+1] should be conjugate
			b[1]=zzeros[i].plus(zzeros[i+1]).negate();
			b[0]=zzeros[i].multiply(zzeros[i+1]);
	
			//should all be real numbers
			for (var j=0;j<b.length;j++)
				biquadb[biquadb.length]=b[j].r;
		}
		if (nzpoles&1)
		{
			biquada[biquada.length]=-zpoles[nzpoles-1].r;
			biquadb[biquadb.length]=-zzeros[nzzeros-1].r;
		}
	}
}

function updateitem(name,fromsldr)
{
	var v;
	if (fromsldr)
	{
		v=parseFloat(GetObject(name+"slider").value);
		GetObject(name).value=v;
	}
	else
	{
		var o=GetObject(name)
		v=parseFloat(o.value);
		if (v<o.min) v=parseFloat(o.min);
		if (v>o.max) v=parseFloat(o.max);
		GetObject(name+"slider").value=v;
	}
	return v;
}

function updatesldr()
{
	var sr=srate;
	srate=updateitem("samplerate",true);
	cutoff=updateitem("cutoff",true);
	rippledb=updateitem("ripple",true);
	sbrippledb=updateitem("sbripple",true);
	bandwidth=updateitem("bandwidth",true);
	order=updateitem("order",true);
	if (sr!=srate)
	{
		GetObject("cutoffslider").max=String(srate/2);
		GetObject("cutoff").max=String(srate/2);
		cutoff=Math.round(cutoff*srate/sr);
		GetObject("cutoffslider").value=cutoff;
		GetObject("cutoff").value=cutoff;
		GetObject("bandwidthslider").max=String(srate/2);
		GetObject("bandwidth").max=String(srate/2);
		bandwidth=Math.round(bandwidth*srate/sr);
		GetObject("bandwidthslider").value=bandwidth;
		GetObject("bandwidth").value=bandwidth;
	}

	plot();
}

function updatectl()
{
	var sr=srate;
	srate=updateitem("samplerate",false);
	cutoff=updateitem("cutoff",false);
	rippledb=updateitem("ripple",false);
	sbrippledb=updateitem("sbripple",false);
	bandwidth=updateitem("bandwidth",false);
	order=updateitem("order",false);
	if (sr!=srate)
	{
		GetObject("cutoffslider").max=String(srate/2);
		GetObject("cutoff").max=String(srate/2);
		cutoff=Math.round(cutoff*srate/sr);
		GetObject("cutoffslider").value=cutoff;
		GetObject("cutoff").value=cutoff;
		GetObject("bandwidthslider").max=String(srate/2);
		GetObject("bandwidth").max=String(srate/2);
		bandwidth=Math.round(bandwidth*srate/sr);
		GetObject("bandwidthslider").value=bandwidth;
		GetObject("bandwidth").value=bandwidth;
	}

	codetype=GetObject("codetype").value;
	filtertype=GetObject("filtertype").value;
	filtermode=GetObject("filtermode").value;
	GetObject("ripplerow").style.display=(filtertype=="c" || filtertype=="e" || filtertype=="a")?"table-row":"none";
	GetObject("sbripplerow").style.display=(filtertype=="e" || filtertype=="i")?"table-row":"none";
	GetObject("bandwidthrow").style.display=(filtermode=="band" || filtermode=="notch")?"table-row":"none";
	plot();
}

function plot()
{
	filtertype=GetObject("filtertype").value;
	filtermode=GetObject("filtermode").value;
	plottype=GetObject("plottypez").checked?"z":"s";
	plotfreq=GetObject("plotfreq").value;
	plotphase=GetObject("plotphase").checked;
	plothphase=GetObject("plothphase").checked;
	plotgdel=GetObject("plotgdel").checked;
	plotsfreq=GetObject("plotsfreq").checked;
	filterinput=GetObject("filterinput").value;
	inputoctave=GetObject("inputoctave").value;
	showinput=GetObject("showinput").checked;
	playsound=GetObject("playsound").checked;

log.msg="";
	clear();
	switch (filtertype)
	{
	case "b":
		butterworth(order, cutoff);
		break;
	case "c":
		chebyshev(order, cutoff,rippledb);
		break;
	case "i":
		chebyshev2(order, cutoff,sbrippledb);
		break;
	case "e":
		elliptic(order,cutoff,rippledb,sbrippledb);
		break;

	}
	switch (filtermode)
	{
	case "low":
		break;
	case "high":
		makehighpass();
		break;
	case "band":
		makebandpass();
		break;
	case "notch":
		makenotch();
		break;
	}
	GetObject("splaneplot").style.display=(plottype=="s")?"inline":"none";
	GetObject("zplaneplot").style.display=(plottype=="z")?"inline":"none";
	drawsplane();
	drawzplane();
	response(rctx);
	getcoeffs();
	filter(cctx);
	playaudio();

	getcode();
	
	saveparams();
}

function playaudio()
{
	if (audioContext==null)
	{
		if (GetObject("playsound").checked)
			alert("Sorry, audio not supported in this browser");
		GetObject("playsound").checked=false;
		return;
	}
	playsound=GetObject("playsound").checked;
	localStorage.playsound=playsound;
	if (playsound)
	{
		waveGain.gain.value = 0.2;
	}
	else
	{
		waveGain.gain.value = 0;
	}
}

function getcode()
{
	getcoeffs();
	var code="";
	if (codetype=="n")
	{
		code+="typedef double REAL;\n";
		code+="#define NPOLE "+nzpoles+"\n";
		code+="#define NZERO "+nzzeros+"\n";
		code+="REAL acoeff[]={"+acoeffs.join(",")+"};\n";
		code+="REAL bcoeff[]={"+bcoeffs.join(",")+"};\n";
		code+="REAL gain="+gain+";\n";
		var zr="0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
		code+="REAL xv[]={"+(zr.substr(0,nzzeros*2+1))+"};\n";
		code+="REAL yv[]={"+(zr.substr(0,nzpoles*2+1))+"};\n";
		code+="\n";
		code+="REAL applyfilter(REAL v)\n";
		code+="{\n";
		code+="\tint i;\n";
		code+="\tREAL out=0;\n";
		code+="\tfor (i=0; i&lt;NZERO; i++) {xv[i]=xv[i+1];}\n";
		code+="\txv[NZERO] = v/gain;\n";
		code+="\tfor (i=0; i&lt;NPOLE; i++) {yv[i]=yv[i+1];}\n";
		code+="\tfor (i=0; i&lt;=NZERO; i++) {out+=xv[i]*bcoeff[i];}\n";
		code+="\tfor (i=0; i&lt;NPOLE; i++) {out-=yv[i]*acoeff[i];}\n";		
		code+="\tyv[NPOLE]=out;\n";
		code+="\treturn out;\n";
		code+="}\n";
	}
	else
	{
		code+="typedef double REAL;\n";
		var nbq=((nzpoles+1)>>1);
		var nxy=(3+3*((nzpoles+1)>>1));
		code+="#define NBQ "+nbq+"\n";
		code+="REAL biquada[]={"+biquada.join(",")+"};\n";
		code+="REAL biquadb[]={"+biquadb.join(",")+"};\n";
		code+="REAL gain="+gain+";\n";
		var zr="0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
		code+="REAL xyv[]={"+(zr.substr(0,nxy*2-1))+"};\n";
		code+="\n";
		code+="REAL applyfilter(REAL v)\n";
		code+="{\n";
		code+="\tint i,b,xp=0,yp="+(nzzeros>1?3:2)+",bqp=0;\n";
		code+="\tREAL out=v/gain;\n";
		code+="\tfor (i="+(nxy-1)+"; i&gt;0; i--) {xyv[i]=xyv[i-1];}\n";
		code+="\tfor (b=0; b&lt;NBQ; b++)\n";
		code+="\t{\n";
		code+="\t\tint len="+((nzzeros&1)?("(b==NBQ-1)?1:2"):("2"))+";\n";
		code+="\t\txyv[xp]=out;\n";
		code+="\t\tfor(i=0; i&lt;len; i++) { out+=xyv[xp+len-i]*biquadb[bqp+i]-xyv[yp+len-i]*biquada[bqp+i]; }\n"
		code+="\t\tbqp+=len;\n";
		code+="\t\txyv[yp]=out;\n";
		code+="\t\txp=yp; yp+=len+1;\n";
		code+="\t}\n";
		code+="\treturn out;\n";
		code+="}\n";
	}
	GetObject("codebody").innerHTML="<PRE>"+code+"</PRE>";
}


restoreparams();
updatectl();
-->
</script>

</div>
<div id="bottomad">
<script async src="//pagead2.googlesyndication.com/pagead/js/adsbygoogle.js"></script>
<!-- JaggedPlanet Responsive -->
<ins class="adsbygoogle"
     style="display:block"
     data-ad-client="ca-pub-7472547808115475"
     data-ad-slot="6629101740"
     data-ad-format="auto"></ins>
<script>
(adsbygoogle = window.adsbygoogle || []).push({});
</script>
</div>
<div id="pagefooter">
<DIV style="float:right;">
<A href="/contact.asp">Contact</A> &nbsp;  &nbsp;  &nbsp; &nbsp;
<A href="/sitemap.asp">Site map</A> &nbsp;  &nbsp;  &nbsp; &nbsp;
<A href="/privacy.asp">Privacy policy</A> &nbsp;  &nbsp;  &nbsp; &nbsp;
<BR>&nbsp;<BR>
</DIV>
Copyright &copy; 2020 JaggedPlanet.com
</div>

<script>
  (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
  (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
  m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
  })(window,document,'script','//www.google-analytics.com/analytics.js','ga');

  ga('create', 'UA-65853812-2', 'auto');
  ga('send', 'pageview');

</script>


</body>
</html>