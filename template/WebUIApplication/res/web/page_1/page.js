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
		this.#TargetTableElement = document.getElementById("로그표");
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
		row.cells[1].width = "150";
		row.cells[2].width = "100";
			
		row.cells[3].className = "left-align";

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
class ConsoleMessageWindow {
	
	//-----------------------------------------------------------------------
	#TargetDivElement = null;
	#ConsoleMessageCount = null;

	//-----------------------------------------------------------------------
	constructor() {
		this.#TargetDivElement = document.getElementById("내용콘솔메시지창");
		this.#ConsoleMessageCount = 0;
	}

	//-----------------------------------------------------------------------
	getDateTimeString() {
		const now = new Date();
		
		const year    = now.getFullYear      ();
		const month   = String(now.getMonth  () + 1).padStart(2, '0');
		const date    = String(now.getDate   ()    ).padStart(2, '0');
		const hours   = String(now.getHours  ()    ).padStart(2, '0');
		const minutes = String(now.getMinutes()    ).padStart(2, '0');
		const seconds = String(now.getSeconds()    ).padStart(2, '0');
		
		const dateString = `${year}-${month}-${date}`;
		const timeString = `${hours}:${minutes}:${seconds}`;

		let dateTimeString;
		dateTimeString = dateString + " " + timeString;
		return dateTimeString;
	}

	addMessage(stringMessage, messageTextColor = "black") {
		let textString;
		textString = this.getDateTimeString();
		textString += " ";
		textString += stringMessage;


		let targetElement = this.#TargetDivElement;
		if (targetElement == null) {
			return;
		}
		

		if (0==(this.#ConsoleMessageCount%10))
		{
			targetElement.innerHTML = "";
		}


		targetElement.innerHTML += `<span style="color:${messageTextColor};">${escapeHtml(textString)}</span><br/>`;


		targetElement.scrollTop = targetElement.scrollHeight;
		//targetElement.scrollIntoView({ behavior: "instant", block: "end" });


		this.#ConsoleMessageCount++;
	}
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Page extends BasePage {

	//-----------------------------------------------------------------------
	_ConsoleMessageWindow = null;

	//-----------------------------------------------------------------------
	constructor() {
		super();
		this.setup();
	}

	//-----------------------------------------------------------------------
	setup() {
		this.initializeCommandHandlerMap();
		this.setupWebMessageHandler();


		this.setupClickEventListener("페이지1", "postPageNavigation", "집");
		this.setupClickEventListener("메시지1", "postStringMessage", "JS문자열");
		

		let jsonFileTable = new JsonFileTable();
		jsonFileTable.update("/file.json");


		this._ConsoleMessageWindow = new ConsoleMessageWindow();
	}


	//-----------------------------------------------------------------------
	initializeCommandHandlerMap() {
		this._CommandHandlerMap = new Map();
		this._CommandHandlerMap.set("메시지", this.onStringMessageCommand.bind(this));
		this._CommandHandlerMap.set("페이지이동", this.onNavigateCommand.bind(this));
	}

	onNavigateCommand(argData) {
		alert(argData.TargetPage);
	}

	onStringMessageCommand(argData) {
		const colors = ["red", "green", "blue", "orange", "purple", "brown", "pink"];
		const randomColor = colors[Math.floor(Math.random() * colors.length)];

		
		let i;
		for(i=0; i<3; i++)
		{
			this._ConsoleMessageWindow.addMessage(argData.StringMessage, randomColor);
		}


		alert(argData.StringMessage);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
var _Page = null;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
window.onload = function () {
	_Page = new Page();
}


