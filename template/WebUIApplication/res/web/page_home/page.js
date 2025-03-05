﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
"use strict";





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Page extends BasePage {

	//-----------------------------------------------------------------------
	constructor() {
		super();
		this.setup();
	}

	//-----------------------------------------------------------------------
	setup() {
		this.setupClickEventListener("페이지1", "postPageNavigation", "페이지1");
		this.setupClickEventListener("페이지2", "postPageNavigation", "페이지2");
		this.setupClickEventListener("페이지3", "postPageNavigation", "페이지3");
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


