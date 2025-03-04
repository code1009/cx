/////////////////////////////////////////////////////////////////////////////
//===========================================================================
"use strict";





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
function escapeHtml(str) {
	const map = {
		'&': '&amp;',
		'<': '&lt;',
		'>': '&gt;',
		'"': '&quot;',
		"'": '&#39;'
	};
	return str.replace(/[&<>"']/g, function (m) { return map[m]; });
}

function unescapeHtml(str) {
	const map = {
		'&amp;': '&',
		'&lt;': '<',
		'&gt;': '>',
		'&quot;': '"',
		'&#39;': "'"
	};
	return str.replace(/&(amp|lt|gt|quot|#39);/g, function (m) { return map[m]; });
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class JsonFileTable {
	
	//-----------------------------------------------------------------------
	constructor() {
	}
	
	//-----------------------------------------------------------------------
	update(urn) {
		var xmlHttpRequest = new XMLHttpRequest();


		xmlHttpRequest.onreadystatechange =
			function () {
				if (xmlHttpRequest.readyState == 4 && xmlHttpRequest.status == 200) {
					let myObj = JSON.parse(this.responseText);
					//this.updateTable(myObj);
					let jsonFileTable = new JsonFileTable();
					jsonFileTable.updateTable(myObj);
				}
			};

		xmlHttpRequest.open("GET", urn, true);
		xmlHttpRequest.setRequestHeader("Content-type", "application/json");
		xmlHttpRequest.send();
	}

	//-----------------------------------------------------------------------
	updateTable(data) {
		this.clearTable();

		if (0 != data.length) {
			for (let i = 0; i < data.length; i++) {
				this.addRow(i, data[i]);
			}
		}
	}

	clearTable() {
		let targetElement;
		targetElement = document.getElementById('내용정보표');

		while (targetElement.rows.length > 1) {
			targetElement.deleteRow(-1);
		}
	}
	
	
	addRow(i, data) {
		let targetElement;
		let row;


		targetElement = document.getElementById('내용정보표');

		row = targetElement.insertRow(-1);
		//row = targetElement.insertRow(1);

		row.insertCell(0); row.cells[0].width = "50";
		row.insertCell(1); row.cells[1].width = "100";
		row.insertCell(2); row.cells[2].width = "100";
		row.insertCell(3);
	
		/*
		row.cells[0].className = "표형식";
		row.cells[1].className = "표형식";
		row.cells[2].className = "표형식";
		row.cells[3].className = "표형식";
		*/
		
		this.setRow(i, row, data);
	}
	
	setRow(i, row, data) {
		row.cells[0].innerHTML = escapeHtml(data[0]);
		row.cells[1].innerHTML = escapeHtml(data[1]);
		row.cells[2].innerHTML = escapeHtml(data[2]);
		row.cells[3].innerHTML = escapeHtml(data[3]);
	}
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Page {

	//-----------------------------------------------------------------------
	#Context = null;

	//-----------------------------------------------------------------------
	constructor() {
		this.#Context = null;
		this.setup();
	}

	//-----------------------------------------------------------------------
	setup() {
		this.setupWebMessageHandler();
		this.setupPostNavigateButton("페이지이동-페이지1", "집");
		this.setupPostMessageButton("메시지1", "JS문자열");
		
		let jsonFileTable = new JsonFileTable();
		jsonFileTable.update("/file.json");
	}

	//-----------------------------------------------------------------------
	setupPostNavigateButton(targetName, page) {
		let targetElement;

		targetElement = document.getElementById(targetName);
		if (targetElement != null) {
			targetElement.addEventListener(
				"click",
				(e) => {
					this.postNavigatePage(page);
				}
			);
		}
	}

	postNavigatePage(page){
		let jsonMessage =
		{
			Command: "페이지이동",
			TargetPage: page
		};
		_Core.contentsPostMessage(jsonMessage);
	}

	//-----------------------------------------------------------------------
	setupPostMessageButton(targetName, messageString) {
		let targetElement;

		targetElement = document.getElementById(targetName);
		if (targetElement != null) {
			targetElement.addEventListener(
				"click",
				(e) => {
					this.postMessageString(messageString);
				}
			);
		}
	}

	postMessageString(messageString){
		let jsonMessage =
		{
			Command: "메시지",
			MessageString: messageString
		};
		_Core.contentsPostMessage(jsonMessage);
	}
	
	//-----------------------------------------------------------------------
	setupWebMessageHandler() {
		window.chrome.webview.addEventListener(
			"message",
			arg => {
				this.onWebMessage(arg);
			}
		);
	}

	onWebMessage(arg) {
		if ("Command" in arg.data)
		{
			this.onCommand(arg.data);
		}
	}
	
	onCommand(argData) {
		if ("메시지"==argData.Command) {
			this.onMessageStringCommand(argData);
		}
		else {
			
		}
	}

	onMessageStringCommand(argData) {
		alert(argData.MessageString);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
var _Page = null;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
window.onload = function () {
	coreInitialize();
	_Page = new Page();
}


