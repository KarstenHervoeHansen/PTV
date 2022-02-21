/*
 *  DO NOT DELETE THIS FILE
 *  Copyright 2006 Baynote, Inc. All Rights Reserved
 *  Date: September 14, 2006
 *
 *
 *  1. Replace previous baynote.js with this file
 *  2. To enable A/B testing, set AB_TEST = "true"
 *  3. Edit AD_MARKERS if any paid-ads 
 *  4. When testing period is over, set AB_TEST = "false"
 *
 */

var AB_TEST    = "false";

var AD_MARKERS = "";  // For Paid-Ads.
                      // If any, fill in with 
                      //   comma-delimited ad marker key-value pairs
                      // These k-v pairs are customer-defined per search engine
                      //   eg.  YHOO_FOO=111,GOOG_BAR=222,MSN_BAZ=333



function bntest_setCookie( key, value )
{
    var cookieStr = key + "=" + value;
    cookieStr += "; expires=null";
    cookieStr += "; path=/";
    document.cookie = cookieStr;
}
function bntest_removeCookie( key, value )
{
    var cookieStr = key + "=" + value;
    cookieStr += "; expires=" + new Date(0).toGMTString();
    cookieStr += "; path=/";
    document.cookie = cookieStr;
}
if (AB_TEST == "true")
{
    bntest_setCookie("bn_abtest", AB_TEST);
    bntest_setCookie("bn_admarkers", AD_MARKERS);
}
else
{
    bntest_removeCookie("bn_abtest", null);
    bntest_removeCookie("bn_admarkers", null);
    bntest_removeCookie("bn_abg", null);
    bntest_removeCookie("bn_AdGuide_loc", null);
    bntest_removeCookie("bn_MostPopular_loc", null);
    bntest_removeCookie("bn_NextStep_loc", null);
    bntest_removeCookie("bn_SimilarDocs_loc", null);
    bntest_removeCookie("bn_extq", null);
    bntest_removeCookie("bn_extr", null);
    bntest_removeCookie("bn_extad", null);
}


function _baynote_tag_create()
{
  function _baynote_tag( other)
  {
    if ( other )
    {
      this.server = other.server;
      this.customerId = other.customerId;
      this.code = other.code;
      this.userId = other.userId;
      this.linkTestEnabled = other.linkTestEnabled;
      this.linkTestServerPage = other.linkTestServerPage;
    }
  }

  _baynote_tag.id = 0;
  _baynote_tag.loadlist = new Array();

  _baynote_tag.prototype.initialLoadTime = new Date().getTime();

  var _bn_re = /(^baynote_testing|; baynote_testing)=([^;]*)/i;
  var _bn_match = _bn_re.exec( document.cookie);
  if ( _bn_match != null && _bn_match.length >= 3 )
  {
    var _bn_server = unescape(_bn_match[2]);
    if ( _bn_server && _bn_server != "" )
      _baynote_tag.server = _bn_server;
  }

  _baynote_tag.createLoadCallback = function( src, img)
  {
    return function()
    {
      img.onloadDone = true;

      var script = document.createElement( "script");
      script.setAttribute( "language", "JavaScript");
      script.setAttribute( "type", "text/javascript");
      script.src = src;
      document.body.appendChild( script);
    }
  }

  _baynote_tag.createImgLoadCallback = function( jssrc)
  {
    return function( img)
    {
      var callback = _baynote_tag.createLoadCallback( jssrc, img);
      if ( img.onloadDone == true )
      {
        callback();
      }
      else if (img.attachEvent)
      {
        if ( img.readyState == "complete" )
        {
          callback();
        }
        else
        {
          img.attachEvent("onload", callback);
        }
      }
      else if (img.addEventListener)
      {
        img.addEventListener("load", callback, false);
      }
    }
  }

  _baynote_tag.prototype.show = function( bn_parent)
  {
    var here = new String(window.location);
    if (-1 != here.indexOf("bn_disable=1"))
    {
      // no-op
      return;
    }
    if ( _baynote_tag.server )
      baynote_tag.server = _baynote_tag.server;
    baynote_tag.id = _baynote_tag.id++;
    baynote_tag.baseid = "_baynote_" + baynote_tag.id;
    window[baynote_tag.baseid] = baynote_tag;

    if ( !baynote_tag.type )
      baynote_tag.type = "baynoteObserver";

    var imgid = "_baynote_" + baynote_tag.server + "_" + baynote_tag.customerId + "_" + baynote_tag.code;
    var img = document.getElementById( imgid);
    var base = baynote_tag.server + "/baynote/";
    var path = "/" + baynote_tag.customerId + "-" + baynote_tag.code +
                 "?customerId=" + baynote_tag.customerId + "&code=" + baynote_tag.code;
    var imgsrc = base + "customerstatusjs" + path +
                 "&x=" + baynote_tag.id + new Date().getTime();
    var jssrc = base + "tags/baynotejs" + path + "&id=" + baynote_tag.baseid + "&type=" + baynote_tag.type;

    var placeHolderType = baynote_tag.popup ? "span" : "div";
    if ( baynote_tag.placeholderElement )
      placeHolderType = baynote_tag.placeholderElement;
    if ( bn_parent )
    {
      var placeHolder = document.createElement( placeHolderType);
      placeHolder.id = baynote_tag.baseid + "_placeholder";
      bn_parent.appendChild( placeHolder);
    }
    else
    {
      document.write( "<" + placeHolderType + " id=\"" + baynote_tag.baseid + "_placeholder\"></" + placeHolderType + ">");
    }

    var imgLoadCallback = _baynote_tag.createImgLoadCallback( jssrc);

    if ( !img )
    {
      if ( !_baynote_tag.imgloading )
      {
        img = document.createElement("IMG");
        img.id = imgid;
        img.src = imgsrc;
        img.width = 1;
        img.height = 1;
        img.style.position = "absolute";
        img.style.top = "0px";
        img.style.left = "0px";
        img.style.opacity = 0.0;                  // firefox
        img.style.filter = "alpha(opacity=0)";    // IE
        if ( window.attachEvent && !window.opera )
        {
          window.attachEvent( "onload", function()
          {
            document.body.appendChild( img);
            for ( var i = 0; i < _baynote_tag.loadlist.length; i++)
            {
              _baynote_tag.loadlist[i]( img);
            }
            _baynote_tag.loadlist = new Array();
          });
          _baynote_tag.loadlist[ _baynote_tag.loadlist.length] = imgLoadCallback;
          _baynote_tag.imgloading = true;
        }
        else
        {
          document.body.appendChild( img);
          imgLoadCallback( img);
        }
      }
      else
      {
        _baynote_tag.loadlist[_baynote_tag.loadlist.length] = imgLoadCallback;
      }
    }
    else
    {
      imgLoadCallback( img);
    }

    var createdId = baynote_tag.baseid;
    baynote_tag = new _baynote_tag( baynote_tag);
    return createdId;
  }

  _baynote_tag.prototype.noshow = function()
  {
    baynote_tag = new _baynote_tag( baynote_tag);
  }

  baynote_tag = new _baynote_tag();

  return _baynote_tag;
}

var _baynote_tag = typeof(_baynote_tag) == "undefined" ? _baynote_tag_create() : _baynote_tag;