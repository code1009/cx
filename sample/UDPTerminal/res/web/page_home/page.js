/////////////////////////////////////////////////////////////////////////////
//===========================================================================
"use strict";





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*
class ConsoleMessageWindow {

    //-----------------------------------------------------------------------
    #TargetDivElement = null;
    #ConsoleMessageCount = null;

    //-----------------------------------------------------------------------
    constructor() {
        this.#TargetDivElement = document.getElementById("내용콘솔메시지창");
        this.#ConsoleMessageCount = 0;

        this.processMessageQueue();
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
        const milliseconds = String(now.getMilliseconds()).padStart(3, '0');

        const dateString = `${year}-${month}-${date}`;
        const timeString = `${hours}:${minutes}:${seconds}.${milliseconds}`;

        let dateTimeString;
        dateTimeString = dateString + " " + timeString;
        return dateTimeString;
    }

    addMessage(stringMessage, messageTextColor = "black") {
        let textString;
        
        //textString = this.getDateTimeString();
        //textString += " ";
        //textString += stringMessage;
        
        textString = stringMessage;
    
        let targetElement = this.#TargetDivElement;
        if (targetElement == null) {
            return;
        }
    
    
        if (0 == (this.#ConsoleMessageCount % 128)) {
            targetElement.innerHTML = "";
        }
    
        //-------------------------------------------------------------------
        //targetElement.innerHTML += `<span style="color:${messageTextColor};">${escapeHtml(textString)}</span><br/>`;
        //-------------------------------------------------------------------
        const fragment = document.createDocumentFragment();
        const span = document.createElement("span");
        span.style.color = messageTextColor;
        span.style.whiteSpace = "nowrap";
        span.innerHTML = escapeHtml(textString);
        fragment.appendChild(span);
        fragment.appendChild(document.createElement("br"));
        targetElement.appendChild(fragment);
        //-------------------------------------------------------------------
    
    
        targetElement.scrollTop = targetElement.scrollHeight;
        //targetElement.scrollIntoView({ behavior: "instant", block: "end" });
    
    
        this.#ConsoleMessageCount++;
    }
}
*/

class ConsoleMessageWindow {

    //-----------------------------------------------------------------------
    #TargetDivElement = null;
    #ConsoleMessageCount = null;

    #MessageQueue = [];
    #BatchSize = 64;

    //-----------------------------------------------------------------------
    constructor() {
        this.#TargetDivElement = document.getElementById("내용콘솔메시지창");
        this.#ConsoleMessageCount = 0;

        this.processMessageQueue();
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
        const milliseconds = String(now.getMilliseconds()).padStart(3, '0');

        const dateString = `${year}-${month}-${date}`;
        const timeString = `${hours}:${minutes}:${seconds}.${milliseconds}`;

        let dateTimeString;
        dateTimeString = dateString + " " + timeString;
        return dateTimeString;
    }

    addMessage(stringMessage, messageTextColor = "black") {
        this.#MessageQueue.push({ stringMessage, messageTextColor });

        if (this.#MessageQueue.length >= this.#BatchSize) {
            this.flushMessages();
        }
    }

    flushMessages() {
        if (this.#MessageQueue.length === 0) {
            return;
        }

        let targetElement = this.#TargetDivElement;
        if (targetElement == null) {
            return;
        }

        const fragment = document.createDocumentFragment();

        this.#MessageQueue.forEach(({ stringMessage, messageTextColor }) => {

            if (0 == (this.#ConsoleMessageCount % 1024)) {
                targetElement.innerHTML = "";
            }

            const span = document.createElement("span");
            span.style.color = messageTextColor;
            span.style.whiteSpace = "nowrap";
            span.innerHTML = escapeHtml(stringMessage);
            fragment.appendChild(span);
            fragment.appendChild(document.createElement("br"));
            this.#ConsoleMessageCount++;
        });

        targetElement.appendChild(fragment);
        targetElement.scrollTop = targetElement.scrollHeight;

        this.#MessageQueue = [];
    }

    processMessageQueue() {
        requestAnimationFrame(() => {
            this.flushMessages();
            this.processMessageQueue();
        });
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

        //document.getElementById("연결").disabled = false;
        //document.getElementById("연결해제").disabled = true;

        this.setupClickEventListener("연결", "connect", "연결");
        this.setupClickEventListener("연결해제", "connect", "연결해제");

        this.setupClickEventListener("송신", "send", "송신");
        this.setupClickEventListener("클립보드복사", "copyClipboard", "클립보드복사");

        this._ConsoleMessageWindow = new ConsoleMessageWindow();
    }

    connect(stringMessage) {
        let laddress = document.getElementById("로컬주소").value;
        let lport = document.getElementById("로컬포트").value;
        let raddress = document.getElementById("원격주소").value;
        let rport = document.getElementById("원격포트").value;

        let jsonMessage =
        {
            Command: stringMessage,
            LocalAddress: laddress,
            LocalPort: lport,
            RemoteAddress: raddress,
            RemotePort: rport
        };
        this.postJsonMessage(jsonMessage);
    }

    send(stringMessage) {
        let txData = document.getElementById("송신데이터").value;

        let jsonMessage =
        {
            Command: stringMessage,
            TxData: txData
        };
        this.postJsonMessage(jsonMessage);
    }

    copyClipboard(stringMessage) {
        const element = document.getElementById("내용콘솔메시지창");

        const text = element.innerText;

        // Create a temporary textarea element to hold the text
        const textarea = document.createElement("textarea");
        textarea.value = text;
        document.body.appendChild(textarea);

        // Select the text and copy it to the clipboard
        textarea.select();
        document.execCommand("copy");

        // Remove the temporary textarea element
        document.body.removeChild(textarea);
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


