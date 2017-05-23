function tgl1() {
	getResponse("tgl1.json", function(responseText) {
		console.log(responseText);
    	if (responseText == "true0") {
    		document.getElementById("b1").className = "button";
   		} else if (responseText == "true1") {
   			document.getElementById("b1").className = "button-primary";
   		}});
}

function tgl2() {
	getResponse("tgl2.json", function(responseText) {
		console.log(responseText);
    	if (responseText == "true0") {
    		document.getElementById("b2").className = "button";
   		} else if (responseText == "true1") {
   			document.getElementById("b2").className = "button-primary";
   		}});
}

function tgl3() {
	getResponse("tgl3.json", function(responseText) {
		console.log(responseText);
    	if (responseText == "true0") {
    		document.getElementById("b3").className = "button";
   		} else if (responseText == "true1") {
   			document.getElementById("b3").className = "button-primary";
   		}});
}

function ir() {
	getResponse("ir.json", function(responseText) {
		console.log(responseText);
	if (responseText == "true") {
   			alert("Sent IR Code!");
   		}});
}

function ir2() {
	getResponse("IRrcd.json", function(responseText) {});
}

function ir3() {
	getResponse("IRsnd.json", function(responseText) {
		console.log(responseText);
	if (responseText == "true") {
   			alert("Sent IR Code!");
   		}});
	}


setInterval(function(){ update(); }, 500);

function update() {
	getResponse("update.json", function(responseText) {
		var strArr = responseText.split(',');
		var intArr = [];
		for(i=0; i < strArr.length; i++)
  	  	intArr.push(parseInt(strArr[i]));

		document.getElementById("temp").innerHTML = intArr[0]+" *C";
		document.getElementById("light").innerHTML = intArr[1];
		document.getElementById("mov").innerHTML = intArr[2]+" mov/min";
		document.getElementById("vbat").innerHTML = intArr[3]+" V";
		document.getElementById("uptime").innerHTML = intArr[4];
		if (intArr[5] == 1) {
			document.getElementById("capt").className = "button-primary";
		} else if (intArr[5] == 0) {
			document.getElementById("capt").className = "button";
		}
		document.getElementById("disp").innerHTML = "Send "+intArr[6];
	});
}
