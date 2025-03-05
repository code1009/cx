/////////////////////////////////////////////////////////////////////////////
//===========================================================================
"use strict";





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class JsonFileTable {
	
	//-----------------------------------------------------------------------
	#TargetTableElement = null;
	#TargetTableCellCount = null;

	//-----------------------------------------------------------------------
	constructor() {
		this.#TargetTableElement = document.getElementById("내용정보표");
		this.#TargetTableCellCount = 4;
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
		targetElement = this.#TargetTableElement;
		while (targetElement.rows.length > 1) {
			targetElement.deleteRow(-1);
		}
	}
	
	addRow(dataRowIndex, dataCell) {
		let targetElement;
		let row;
		targetElement = this.#TargetTableElement;
		row = targetElement.insertRow(-1);
		//row = targetElement.insertRow(1);


		let i;
		for (i=0; i<this.#TargetTableCellCount; i++)
		{
			row.insertCell(i);
		}


		row.cells[0].width = "50";
		row.cells[1].width = "100";
		row.cells[2].width = "100";
			
		/*
		row.cells[0].className = "표형식";
		row.cells[1].className = "표형식";
		row.cells[2].className = "표형식";
		row.cells[3].className = "표형식";
		*/
		
		this.setRow(row, dataRowIndex, dataCell);
	}
	
	setRow(row, dataRowIndex, dataCell) {
		row.cells[0].innerHTML = escapeHtml(dataCell[0]);
		row.cells[1].innerHTML = escapeHtml(dataCell[1]);
		row.cells[2].innerHTML = escapeHtml(dataCell[2]);
		row.cells[3].innerHTML = escapeHtml(dataCell[3]);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Page {

	//-----------------------------------------------------------------------
	#CommandHandlerMap = null;

	//-----------------------------------------------------------------------
	constructor() {
		this.setup();
	}

	//-----------------------------------------------------------------------
	setup() {
		this.initializeCommandHandlerMap();
		this.setupWebMessageHandler();

		this.setupClickEventListener("페이지1", "postNavigatePage", "집");
		this.setupClickEventListener("메시지1", "postMessageString", "JS문자열");
		
		let jsonFileTable = new JsonFileTable();
		jsonFileTable.update("/file.json");
	}

	//-----------------------------------------------------------------------
	setupClickEventListener(targetName, methodName, methodParam) {
		let targetElement = document.getElementById(targetName);
		if (targetElement != null) {
			targetElement.addEventListener(
				"click",
				(e) => {
					if (typeof this[methodName] === 'function') {
						this[methodName](methodParam);
					}
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
	
	//-----------------------------------------------------------------------
	initializeCommandHandlerMap() {
		this.#CommandHandlerMap = new Map();
		this.#CommandHandlerMap.set("메시지", this.onMessageStringCommand.bind(this));
		this.#CommandHandlerMap.set("페이지이동", this.onNavigateCommand.bind(this));
	}

	onCommand(argData) {
		const commandHandler = this.#CommandHandlerMap.get(argData.Command);
		if (commandHandler) {
			commandHandler(argData);
		} 
		else {
			console.warn(`Unknown command: ${argData.Command}`);
		}
	}

	onNavigateCommand(argData) {
		alert(argData.TargetPage);
	}

	getDateTimeString() {
		const now = new Date();
		
		const year = now.getFullYear();
		const month = String(now.getMonth() + 1).padStart(2, '0');
		const date = String(now.getDate()).padStart(2, '0');
		const hours = String(now.getHours()).padStart(2, '0');
		const minutes = String(now.getMinutes()).padStart(2, '0');
		const seconds = String(now.getSeconds()).padStart(2, '0');
		
		const dateString = `${year}-${month}-${date}`;
		const timeString = `${hours}:${minutes}:${seconds}`;

		let dateTimeString;

		dateTimeString = dateString + " " + timeString;

		return dateTimeString;
	}

	onMessageStringCommand(argData) {
		let textString;
		textString = this.getDateTimeString();
		textString += " ";
		textString += argData.MessageString;


		let targetElement = document.getElementById("내용콘솔창");
		if (targetElement == null) {
			return;
		}
		

		//if (0==(this.#ConsoleLine%500))
		//{
		//	targetElement.innerHTML = "";
		//}


		let i;
		for(i=0; i<10; i++)
		{
			targetElement.innerHTML += escapeHtml(textString);
			targetElement.innerHTML += "<br/>"
		}
		

		targetElement.scrollTop = targetElement.scrollHeight;
		//targetElement.scrollIntoView({ behavior: "instant", block: "end" });


		//this.#ConsoleLine++;


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


