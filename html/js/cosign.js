// Determines if a value exists in an array
// based on code from embimedia.com
Array.prototype.inArray = function ( value )
{
    for ( var i=0; i<this.length; i++ ) {
		if ( typeof( value ) == 'object' ) {
			for ( var j=0; j<value.length; j++ ) {
				if ( this[i] === value[j] ) {
					return true;
				}
			}
		} else {
			// Matches identical (===), not just similar (==).
			if ( this[i] === value ) {
				return true;
			}
		}
    }

    return false;
};


//Remember object based on moo.fx by Valerio Proietti 
//and code by Peter-Paul Koch
var Remember = new Object();
Remember = function(){};

Remember.prototype = {
	setCookie: function( name, value ) {
		document.cookie = name + '=' + value + '; expires=Sun, 17 Jan 2038 12:34:56 UTC; path=/';
	},

	readCookie: function( name ) {
		var nameEQ = name + "=";
		var ca = document.cookie.split( ';' );
		for( var i=0; i<ca.length; i++ ) {
			var c = ca[i];
			while ( c.charAt( 0 )==' ' ) c = c.substring( 1, c.length );
			if ( c.indexOf( nameEQ ) == 0 ) {
				return {data: c.substring( nameEQ.length, c.length ), isset: true};
			}
		}

		return {data: '', isset: false};
	}
}

var Cosign = Class.create();
Cosign.prototype = Object.extend( new Remember(), {
	initialize: function( requiredFctrs ) {
		this.factorCookie    = 'exposedFactors';
		this.attribsCookie   = 'miscAttributes';
		this.factors         = Array();
		this.infoBoxes       = Array();
		this.requiredFctrs   = Array();
		this.cookieFactors   = Array();
		this.cookieAttribs   = Array();
		this.satisfied       = Array();
		this.defaultOpen     = Array();
		this.disabledFactors = Array();
		this.requiredFctrs   = requiredFctrs.split( ',' );
		var cookieFactors    = this.readCookie( this.factorCookie );
		this.cookieFactors   = cookieFactors.data.split( ',' );
		this.factorCookieSet = cookieFactors.isset && this.cookieFactors.max().length > 0;
		var cookieAttribs    = this.readCookie( this.attribsCookie );
		this.cookieAttribs   = cookieAttribs.data.split( ',' );
	},

	setSatisfied: function( satisfied ) {
		this.satisfied = satisfied.split( ',' );
	},

	setDisabledFactors: function( disabled_factor ) {
		this.disabledFactors = disabled_factor.split( ',' );
	},

	setDefaultOpen: function( defaults ) {
		this.defaultOpen = defaults.split( ',' );
	},

	addFactor: function ( factorName, title, factorDivID ) {
		var focusBox = $( factorDivID ).getElementsByTagName( 'input' )[0];
		if ( focusBox.value != '' && focusBox.type != 'checkbox' ) {
			focusBox = $( factorDivID ).getElementsByTagName( 'input' )[1];
		}

		if ( this.factors.select( function(f) {return f.title == title;}).length == 0 )
		{
			this.factors.push({factorName: factorName, title: title, factorDivID: factorDivID, focusBox: focusBox});
		}
	},

	addInfoBox: function ( infoID, infoShow, infoHide ) {
		if ( this.infoBoxes.select( function(i) {return i.infoShow == infoShow;}).length == 0 )
		{
			this.infoBoxes.push( {infoID: infoID, infoShow: infoShow, infoHide: infoHide} );
		}
	},
	
	setSubmitLink: function ( elName, formName ) {
		var submitLink = $( elName );
		Event.observe( submitLink, 'click', function() {$( formName ).submit()}, false );

		// This is old school, but Event.stop will not work in Safari as of 2.0.3...
		submitLink.onclick = function() {return false};
	},

	hide: function( factor ) {
		Effect.BlindUp( factor.factorDivID, {duration: 0.25} );
		$( factor.title ).className = 'dink';
	},

	showSatisfied: function( factor ) {
		Effect.BlindUp( factor.factorDivID, {duration: 0.25} );
		$( factor.title ).className = 'check';
		var nodes  = $( factor.title ).parentNode.childNodes;

		$A( nodes ).each(
			function( node ) {
				if ( node.className == 'authComplete' ) {
					Element.show( node );
				}
			}
		);
	},

	giveVisible: function( focusBox ) {
		try {
			parent = $( focusBox ).up('div')
			if (!parent.visible()) new Effect.Appear(parent,{duration:0.20});
		} catch ( e ) {;}
	},

	giveFocus: function( focusBox ) {
		this.giveVisible( focusBox );
		try {
			$( focusBox ).focus();
			return true;
		} catch ( e ) {
			return false;
		}
	},

	toggle: function( factor ) {
		var blindStart = function() {
			if ( $( factor.factorDivID ).style.display == 'none' ) {
				$( factor.title ).className = 'dinkDown';
			} else {
				$( factor.title ).className = 'dink';
			}
		};

		var blindFinish = function() {
			this.saveFactorState( factor );
			this.closeInfo();
		}.bind(this);

		Effect.toggle( factor.factorDivID, 'blind', {duration: 0.25, beforeStart: blindStart, afterFinish: blindFinish} );
	},

	// Based on http://www.howtocreate.co.uk/jslibs/capsDetect.js
	capsDetect: function ( e ) {
		if( !e ) {
			e = window.event;
		}

		if( !e ) {
			this.MWJ_say_Caps( false );
			return;
		}

		//what (case sensitive in good browsers) key was pressed
		var theKey = e.which ? e.which : ( e.keyCode ? e.keyCode : ( e.charCode ? e.charCode : 0 ));
		//if upper case, check if shift is not pressed. if lower case, check if shift is pressed
		this.MWJ_say_Caps(( theKey > 64 && theKey < 91 && ! e.shiftKey ) || ( theKey > 96 && theKey < 123 && e.shiftKey ));
	},

	MWJ_say_Caps: function( oC ) {
		var elements = $A( document.getElementsByClassName( 'capsLock' ) );
		if( oC ) {
			elements.each( function( node ) {Element.show( node )} );
		} else {
			elements.each( function( node ) {Element.hide( node )} );
		}
	},

	initUI: function() {
		var oThis    = this;
		var focusSet = false;
		
		// Don't set focus if user has already clicked or typed
		Event.observe( window, 'click', function() {focusSet = true} );
		Event.observe( window, 'keydown', function() {focusSet = true} );

		// escape framesets
		if ( window != top ) {
			top.location.href = location.href;
		}

		if ( $( 'error' ) != null ) {
			error_div = $( 'error' )
			if ( error_div.innerHTML.strip() )  {
				if ( ! error_div.visible() )
					error_div.show();
				new Effect.Highlight( error_div );
			}
		}
		
		var nodeList  = document.getElementsByTagName( 'input' );
		var nodes     = $A( nodeList );

		nodes.each( function( node ) {
				// The Scriptaculous Event.Observe method cannot be used here.
				// Most browsers will not capture the event properly with it.
				if ( node.type == 'password') {
					node.onkeypress = function( e ){oThis.capsDetect( e )};
					node.setAttribute( 'autocomplete', 'off' );
				} else if ( node.type == 'text' ) {
					node.setAttribute( 'autocomplete', 'off' );
				}
			});

		// Set factor visibility
		(function () {
			var fades = Array();
			var shows = Array();
			oThis.factors.each( function( factor ) {
				if ( oThis.disabledFactors.inArray( factor.factorName ) ) {
					disable_node = $( factor.factorDivID );
					if (disable_node && disable_node.visible()) { fades.push( Effect.Fade( disable_node ) ); };
					disable_node = disable_node.previous();
					if (disable_node && disable_node.visible()) { fades.push( Effect.Fade( disable_node ) ); };
				} else if (factor.factorDivID) {
					enable_node = $( factor.factorDivID ).previous();
					if (enable_node && !enable_node.visible()) { shows.push( Effect.Appear( enable_node ) ); };
				}
			});
			new Effect.Parallel( fades, {duration: 0.20, afterFinish:function() { new Effect.Parallel( shows, {duration: 0.20 } ); } } )
		})();
		// Set factor focus
		oThis.factors.each( function( factor ) {
			if ( this.satisfied.inArray( factor.factorName )) {
				this.showSatisfied( factor );
			} else if ( this.requiredFctrs.inArray( factor.factorName )) {
				$( factor.title ).className = 'required';
				if ( ! focusSet ) {
					focusSet = this.giveFocus( factor.focusBox );
				} else {
					this.giveVisible( factor.focusBox );
				}
			} else if ( this.factorCookieSet && this.cookieFactors.inArray( factor.factorName )) {
				Event.observe(factor.title, 'click', function(){oThis.toggle(factor)});
				if ( ! focusSet ) {
					focusSet = this.giveFocus( factor.focusBox );
				} else {
					this.giveVisible( factor.focusBox );
				}
			} else if ( this.defaultOpen.inArray( factor.factorName ) && this.factorCookieSet == false ) {
				Event.observe(factor.title, 'click', function(){oThis.toggle(factor)});
				if ( ! focusSet ) {
					focusSet = this.giveFocus( factor.focusBox );
				} else {
					this.giveVisible( factor.focusBox );
				}

				// Set a cookie for the default open factor
				this.saveFactorState( factor );
			} else {
				Event.observe(factor.title, 'click', function(){oThis.toggle(factor)});
				this.hide( factor );
			}
		}.bind( this ));

		// Register InfoBox click events
		this.infoBoxes.each( function( infoBox ) {
			Event.observe(infoBox.infoShow, 'click', function(){oThis.showInfo(infoBox.infoID)});
			Event.observe(infoBox.infoHide, 'click', function(){oThis.closeInfo()});
			Element.hide( infoBox.infoID );
			$( infoBox.infoID ).style.visibility = 'visible'; // Prevents a "flash" of all infoboxes on load
		});

		// Apply misc element attributes
		for ( var i=0; i<this.cookieAttribs.length; i++ ) {
			var attrib = this.cookieAttribs[i].split( '~' );
			if ( attrib.length == 3 ) {
				var el = $( attrib[0] );
				if ( el != null ) {
					el.setAttribute( attrib[1], attrib[2] );
				}
			}
		}
	},

	saveFactorState: function( factor ) {
		if ( $( factor.factorDivID ).style.display != 'none' && ! this.cookieFactors.inArray( factor.factorName )) {
			if ( typeof( factor.factorName ) == 'object' ) {
				this.cookieFactors.push( factor.factorName[0] ); // Some factors have multiple names
			} else {
				this.cookieFactors.push( factor.factorName );
			}
		} else if ( typeof( factor.factorName ) == 'object' ) {
			for ( var i=0; i<this.cookieFactors.length; i++ ) {
				if ( factor.factorName.inArray( this.cookieFactors[i] )) {
					this.cookieFactors.splice( i, 1 );
				}
			}
		} else {
			for ( var i=0; i<this.cookieFactors.length; i++ ) {
				if ( this.cookieFactors[i] == factor.factorName ) {
					this.cookieFactors.splice( i, 1 );
				}
			}
		}

		this.setCookie( this.factorCookie, this.cookieFactors.join());
	},

	saveAttribute: function( id, atName, atValue ) {
		for ( var i=0; i<this.cookieAttribs.length; i++ ) {
			if ( this.cookieAttribs[i].indexOf( id + '~' + atName ) != -1 ) {
				this.forgetAttribute( id, atName );
			}
		}

		this.cookieAttribs.push( id + '~' + atName + '~' + atValue );
		this.setCookie( this.attribsCookie, this.cookieAttribs.join());
	},

	forgetAttribute: function( id, atName ) {
		for ( var i=0; i<this.cookieAttribs.length; i++ ) {
			if ( this.cookieAttribs[i].indexOf( id + '~' + atName ) != -1 ) {
				this.cookieAttribs.splice( i, 1 );
			}
		}

		this.setCookie( this.attribsCookie, this.cookieAttribs.join());
	},

	closeInfo: function() {
		var fades = Array();
		this.infoBoxes.each( function(infoBox ) {
			fades.push( Effect.Fade( infoBox.infoID ));
		});

		new Effect.Parallel( fades, {duration: 0.20, afterFinish:function() { if($('mainInfo')!=null) Effect.Appear('mainInfo',{duration:0.20}); }} );
	},

	showInfo: function( showInfoID ) {
		var fades = Array();
		this.infoBoxes.each( function( infoBox ) {
			if ( infoBox.infoID != showInfoID ) {
				fades.push( Effect.Fade( infoBox.infoID ));
			}
		});

		if ( $('mainInfo') != null ) { fades.push(Effect.Fade('mainInfo')); }

		new Effect.Parallel(fades,{duration: 0.20, afterFinish: function(){Effect.Appear(showInfoID,{duration: 0.20});}});
		
	},

	enDisableCheck: function( trigger, inputEl, label ) {
		var trigger = $( trigger );

		if ( trigger.checked ) {
			$( inputEl ).setAttribute( 'disabled', 'disabled' );
			$( inputEl ).value='';
			$( label ).setAttribute( 'disabled', 'disabled' ); // ie
			$( label ).setAttribute( 'style', 'color: #999' ); // others
			this.saveAttribute( inputEl, 'disabled', 'disabled' );
			this.saveAttribute( label, 'disabled', 'disabled' );
			this.saveAttribute( label, 'style', 'color: #999' );
			this.saveAttribute( trigger.id, 'checked', 'checked' );
		} else {
			$( inputEl ).removeAttribute( 'disabled' );
			$( label ).removeAttribute( 'disabled' ); // ie
			$( label ).removeAttribute( 'style' ); // others
			this.forgetAttribute( inputEl, 'disabled' );
			this.forgetAttribute( label, 'disabled' );
			this.forgetAttribute( label, 'style' );
			this.forgetAttribute( trigger.id, 'checked' );
		}
	},

	enDisableKey: function( trigger, inputEl, label ) {
		var trigger = $( trigger );

		if ( trigger.value.length > 0 ) {
			$( inputEl ).setAttribute( 'disabled', 'disabled' );
			$( label ).setAttribute( 'disabled', 'disabled' ); // ie
			$( label ).setAttribute( 'style', 'color: #999' ); // others
		} else {
			$( inputEl ).removeAttribute( 'disabled' );
			$( label ).removeAttribute( 'disabled' ); // ie
			$( label ).removeAttribute( 'style' ); // others
		}
	}
});

// vim: noet ts=4 sw=4
