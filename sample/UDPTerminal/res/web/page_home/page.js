/////////////////////////////////////////////////////////////////////////////
//===========================================================================
"use strict";





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

    addMessage(stringMessage, messageTextColor = "black") {
        let textString;
        textString = this.getDateTimeString();
        textString += " ";
        textString += stringMessage;


        let targetElement = this.#TargetDivElement;
        if (targetElement == null) {
            return;
        }


        if (0 == (this.#ConsoleMessageCount % 1000)) {
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

        this.setupClickEventListener("연결", "postStringMessage", "연결");
        this.setupClickEventListener("연결해제", "postStringMessage", "연결해제");

        this._ConsoleMessageWindow = new ConsoleMessageWindow();
    }

    postConnect(stringMessage) {
        let address = document.getElementById("주소").value;
        let port = document.getElementById("포트").value;

        let jsonMessage =
        {
            Command: stringMessage,
            Address: address,
            Port: port
        };
        this.postJsonMessage(jsonMessage);
    }

    //-----------------------------------------------------------------------
    initializeCommandHandlerMap() {
        this._CommandHandlerMap = new Map();
        this._CommandHandlerMap.set("메시지", this.onStringMessageCommand.bind(this));
    }

    onStringMessageCommand(argData) {
        this._ConsoleMessageWindow.addMessage(argData.StringMessage, "white");
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


