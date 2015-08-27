/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2015 eduards <<user@hostname.org>>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:element-rtsp_sink
 *
 * FIXME:Describe rtsp_sink here.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m fakesrc ! rtsp_sink ! fakesink silent=TRUE
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif



#include <gst/gst.h>
#include <gst/rtsp/rtsp.h>

#include "gstrtspsink.h"

GST_DEBUG_CATEGORY_STATIC (gst_rtspsink_debug);
#define GST_CAT_DEFAULT gst_rtspsink_debug



#define RTSP_DEFAULT_HOST        "localhost"
#define RTSP_DEFAULT_PORT		1935 
#define RTSP_DEFAULT_STREAM_NAME "live"


/* Filter signals and args */
enum
{
  /* FILL ME */
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_SILENT,
  PROP_HOST,
  PROP_PORT,
  PROP_STREAM_NAME
};

/* the capabilities of the inputs and outputs.
 *
 * describe the real formats here.
 */
static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("application/x-rtp")
    );

/*
application/x-rtp
media : video
	payload : [96, 127]
			  clock - rate : 90000
			  encoding - name : H264
*/
	


#define gst_rtspsink_parent_class parent_class
G_DEFINE_TYPE(GstRTSPsink, gst_rtspsink, GST_TYPE_BASE_SINK);


static void gst_rtspsink_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_rtspsink_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static gboolean gst_rtspsink_sink_event (GstPad * pad, GstObject * parent, GstEvent * event);

static GstFlowReturn  gst_rtsp_sink_preroll(GstBaseSink * bsink, GstBuffer * buffer);

static GstFlowReturn gst_rtsp_sink_render(GstBaseSink * bsink, GstBuffer * buf);
static gboolean default_unroll(GstBaseSink *media);



static GstCaps *
gst_x264_enc_get_supported_input_caps(void)
{
	GstCaps *caps;

	caps = gst_caps_new_simple("application/x-rtp",
		"payload", GST_TYPE_INT_RANGE, 96, 127, NULL);

	//gst_x264_enc_add_x264_chroma_format(gst_caps_get_structure(caps, 0),	x264_chroma_format);

	//GST_DEBUG("returning %" GST_PTR_FORMAT, caps);
	return caps;
}

static GstCaps *
gst_rtp_h264_pay_getcaps(GstBaseSink * payload, GstCaps * pad)
{
	GstCaps *template_caps;
	GstCaps *allowed_caps;
	GstCaps *caps, *icaps;
	gboolean append_unrestricted;
	guint i;
	GstRTSPsink *sink = (GstRTSPsink*)payload;



	GstStructure *structure;
	int rate, channels;
	gboolean ret;
	GstCaps *outcaps;
	structure = gst_caps_get_structure(pad, 0);
	ret = gst_structure_get_int(structure, "payload", &rate);

	//GstCaps *caps;
	//filter->sinkpad = gst_pad_new_from_static_template (&sink_factory, "sink0");

	caps = gst_caps_new_simple("application/x-rtp",
		"payload", GST_TYPE_INT_RANGE, 96, 127, NULL);


	if (!gst_pad_set_caps(pad, caps)) {
		g_printf("Caps negotination failed");
	}


		/*
		application/x-rtp
		media : video
		payload : [96, 127]
		clock - rate : 90000
		encoding - name : H264
		*/

	

//
//	
//	gboolean	b = gst_pad_is_linked(sink->sinkpad);
//
//
//	b = gst_pad_has_current_caps(pad);
//	int j = gst_caps_get_size(caps);
//
//	GstStructure *structure = gst_caps_get_structure(caps, 0);
//
//	gchar *str = gst_structure_get_name(structure);
//
//
////	gst_structure_foreach(structure, print_field, (gpointer)NULL);
//
//
//
//
//	GstCaps *supported_incaps;
//
//
//	supported_incaps = gst_x264_enc_get_supported_input_caps();
//
//	str = gst_caps_get_structure(supported_incaps, 0);
//	gst_structure_get_int(str, "width", &width)

//	//allowed_caps = gst_pad_get_pad_template_caps(sink->sinkpad);
//
//	if (allowed_caps == NULL)
//		return NULL;
//
//	gchar * strrr = gst_caps_to_string(allowed_caps);;
//
//
//	int j = gst_caps_get_size(allowed_caps);
//	template_caps =
//		gst_static_pad_template_get_caps(&sink_factory);
//
//	if (gst_caps_is_any(allowed_caps)) {
//		caps = gst_caps_ref(template_caps);
//		goto done;
//	}
done:
		return caps;
}

/* initialize the rtsp_sink's class */
static void gst_rtspsink_class_init (GstRTSPsinkClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;
  GstBaseSinkClass *gstbase_sink_class;

  gobject_class = (GObjectClass *) klass;
  gstelement_class = (GstElementClass *) klass;
  gstbase_sink_class = GST_BASE_SINK_CLASS(klass);  



  gobject_class->set_property = gst_rtspsink_set_property;
  gobject_class->get_property = gst_rtspsink_get_property;

  g_object_class_install_property (gobject_class, PROP_SILENT, 
	                               g_param_spec_boolean ("silent", "Silent", "Produce verbose output ?",
								   FALSE, G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_HOST,
								  g_param_spec_string("host", "host",
	  							  "The host/IP/Multicast group to send the packets to",
	  							   RTSP_DEFAULT_HOST, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    
  g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_PORT,
								  g_param_spec_int("port", "port", "The port to send the packets to",
								  0, 65535, RTSP_DEFAULT_PORT,
								  G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property(gobject_class, PROP_STREAM_NAME,
								  g_param_spec_string("stream-name", "stream-name",
								  "The stream name ",
								  RTSP_DEFAULT_STREAM_NAME, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));



  gst_element_class_set_details_simple(gstelement_class,
    "RTSPsink",
    "RTSP/UDP sink",
    "RTSP Sink element, it can push stream using RTSP's RECORD capability",
    "Alex Dizengof alex@ireporty.com,  Eduard Sinelnikov eduard@ireporty.com");


  gst_element_class_add_pad_template (gstelement_class,    gst_static_pad_template_get (&sink_factory));

  
  gstbase_sink_class->render = (gst_rtsp_sink_render);

  //query
  gstbase_sink_class->set_caps = gst_rtp_h264_pay_getcaps;

  gstbase_sink_class->preroll = (gst_rtsp_sink_preroll);

}

const int ERROR						= 300;
const int ERR_CONNECTION			= 301;
const int ERR_CANNOT_PUSH_STREAM	= 302;
const int ERR_PARSING				= 303;

int isDigit(char c){
	if (c >= '0' && c <= '9')
		return TRUE;
	else
		return FALSE;
}

int isDigitsOnly(char * str)  
{
	int indx = 0 ; 
	gboolean bAllDigits = TRUE ;
	while (str[indx] != '\0') {

		if (!isDigit(str[indx]))
			bAllDigits = FALSE;

		indx++;
	}

	return bAllDigits;

}



int extractTransportFromMessage(GstRTSPMessage * msg, GstRTSPTransport *transport){
	
	GstRTSPResult res = GST_RTSP_OK;
	GstRTSPHeaderField field = GST_RTSP_HDR_TRANSPORT;
	gchar *value;
	gint indx = 0;
	
	while (res == GST_RTSP_OK) {
		res = gst_rtsp_message_get_header(msg, field, &value, indx);

		if (res == GST_RTSP_OK) {
			res = gst_rtsp_transport_parse(value, transport);
			return GST_RTSP_OK;
		}


		indx++;
	}

	return GST_RTSP_EINVAL;


}

char * extractSessionNumberFromMessage(GstRTSPMessage * msg){


	GstRTSPResult res = GST_RTSP_OK;
	GstRTSPHeaderField field = GST_RTSP_HDR_SESSION;
	gchar *value;
	gint indx = 0;
	gboolean bAnnounceFlag = FALSE, bRecordFlag = FALSE;

	while (res == GST_RTSP_OK) {
		res = gst_rtsp_message_get_header(msg, field, &value, indx);

		// if its only numbers 
		if (isDigitsOnly(value) )
			return (g_strdup(value));
		
		indx++;
	}

	return NULL;


}

static int isServerSupportStreamPush(GstRTSPMessage * msg){
	
	GstRTSPResult res = GST_RTSP_OK;
	GstRTSPHeaderField field = GST_RTSP_HDR_PUBLIC;
	gchar *value;
	gint indx = 0;
	gboolean bAnnounceFlag = FALSE, bRecordFlag = FALSE;

	while (res == GST_RTSP_OK) {
		res = gst_rtsp_message_get_header(msg, field, &value, indx);
		if (0 == g_strcmp0(value, "RECORD"))
			bRecordFlag = TRUE;
		if (0 == g_strcmp0(value, "ANNOUNCE"))
			bAnnounceFlag = TRUE;

		indx++;
	}

	if (bAnnounceFlag != TRUE || bRecordFlag != TRUE) {
		return -ERR_CANNOT_PUSH_STREAM;
	}

	return GST_RTSP_OK;
}


static int isServerReturnOkResponse(GstRTSPMessage  *msg )
{
	GstRTSPResult res;
	GstRTSPStatusCode code;
	const gchar *reason;
	GstRTSPVersion version;

	res = gst_rtsp_message_parse_response(msg, &code, &reason, &version);

	// check if server talks with us
	if (res != GST_RTSP_OK || code != GST_RTSP_STS_OK ) {

		return -ERR_CONNECTION;
	}

	return GST_RTSP_OK;
}


static int sendReceiveAndCheck(GstRTSPConnection *conn, GTimeVal *timeout, GstRTSPMessage  *msg , gboolean debug )
{
	GstRTSPResult res;


	if (debug)
		gst_rtsp_message_dump(msg);

	// set our request
	if ( (res = gst_rtsp_connection_send(conn, msg, timeout))  != GST_RTSP_OK) 
		return -ERR_CONNECTION;


	// get server responce
	if ( (res = gst_rtsp_connection_receive(conn, msg, timeout)) != GST_RTSP_OK)
		return -ERR_CONNECTION;

	if (debug)
		gst_rtsp_message_dump(msg);

	if (isServerReturnOkResponse(msg) != GST_RTSP_OK)
		return -ERR_CANNOT_PUSH_STREAM;

	return GST_RTSP_OK;
}


int setRTPConnectionToServer(GstRTSPsink *sink)
{

	GError *error;

//	gchar *host = sink->host; // "www.ynet.co.il";// "192.168.2.108"; 
//	gint port = sink->server_rtp_port;
//

	if (!sink->socket)  {
		sink->socket = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_DATAGRAM, G_SOCKET_PROTOCOL_UDP, NULL);

		gchar *s;
		GInetAddress *ia;
		ia = g_inet_address_new_from_string(sink->host);


		// Try to get hostby name 
		if (!ia) {
			GResolver *resolver;
			resolver = g_resolver_get_default();
			GList *results;
			results = g_resolver_lookup_by_name(resolver, sink->host, FALSE, &error);
			if (results){
				ia = G_INET_ADDRESS(g_object_ref(results->data));
			}

			gchar *ip = g_inet_address_to_string(ia);

			g_print("IP address for host %s is %s", sink->host, ip);
			g_free(ip);

			g_resolver_free_addresses(results);
			g_object_unref(resolver);
		}


		s = g_inet_address_to_string(ia);
		sink->sa = g_inet_socket_address_new(ia, sink->server_rtp_port);
	}


	if (sink->socket != NULL && sink->sa != NULL)
		return GST_RTSP_OK;

	return GST_RTSP_ERROR;

}


static gboolean print_field(GQuark field, const GValue * value, gpointer pfx) {
	gchar *str = gst_value_serialize(value);

	g_print("---  %15s: %s\n",  g_quark_to_string(field), str);
	g_free(str);
	return TRUE;
}

static GstFlowReturn gst_rtsp_sink_preroll(GstBaseSink * bsink, GstBuffer * buffer)
{
	GstRTSPsink *sink = (GstRTSPsink*)bsink;
	GstRTSPResult res; 
	GstRTSPConnection *conn = sink->conn ;
	GstRTSPUrl * url ;
	GTimeVal timeout;
	guint8 data[4] = {1,2,3,4};
	guint size = 4;
	GstRTSPMessage  msg = {0};
	GstSDPMessage *sdp ;

	//return GST_RTSP_OK;

	const gchar *url_server_str = g_strdup_printf("rtsp://%s", sink->host);  //"rtsp://192.168.2.108"; // TODO: get ip and port from parameters.
	const gchar *url_server_str_full = g_strdup_printf("rtsp://%s:%d/%s", sink->host, sink->port, sink->stream_name);	//"rtsp://192.168.2.108:1935/live/1";
	const gchar *url_server_ip_str = sink->host;// "192.168.2.108";
	const gchar *url_client_ip_str = "0.0.0.0";//"192.168.2.104";
	int port = sink->port;


	timeout.tv_sec = 1; // set timeout to one second.
	timeout.tv_usec = 0;
	sink->user_agent = NULL;// "iReporty\n\0";
	sink->debug = TRUE;
	guint num_ports = 1;
	guint rtp_port = 5006;
	char *szPayloadType = "96"; // TODO: Get payload from sink pad
	char *szSessionNumber; 


	


	// if unrolling close RTSP/TCP connection
	if (bsink->element.current_state == GST_STATE_PLAYING) {
		
		g_print("Unrolling ... ");
		return (default_unroll(bsink));

	}
	
	////////////////////// OPTINS START  //////////////////////////////////////////////////////////


	// set parameters
	res = gst_rtsp_url_parse((const  guint8*)url_server_str, &url);
	res = gst_rtsp_url_set_port(url, port);

	// create connection 
	res = gst_rtsp_connection_create(url, &conn);
	
	res =  gst_rtsp_connection_connect(conn, &timeout);

	if (res != GST_RTSP_OK)
		goto beach;

	GstRTSPMethod method = GST_RTSP_OPTIONS;
	res = gst_rtsp_message_init_request(&msg, method, url_server_str);
	if (res < 0)
		return res;

	/* set user-agent */
	if (sink->user_agent)
		gst_rtsp_message_add_header(&msg, GST_RTSP_HDR_USER_AGENT, sink->user_agent);

	// Send our packet receive server answer and check some basic checks.
	if ( (res = sendReceiveAndCheck(conn, &timeout, &msg, sink->debug)) != GST_RTSP_OK) {
		return res;
	}
	

	// check if server supports RECORD.
	if ( isServerSupportStreamPush(&msg) != GST_RTSP_OK) {
		return -ERR_CANNOT_PUSH_STREAM;
	}

	////////////////////// OPTINS END  //////////////////////////////////////////////////////////



	////////////////////// ANNOUNCE START //////////////////////////////////////////////////////////


	method = GST_RTSP_ANNOUNCE;
	res = gst_rtsp_message_init_request(&msg, method, url_server_str_full);
	if (res < 0)
		return res;

	/* set user-agent */
	if (sink->user_agent)
		gst_rtsp_message_add_header(&msg, GST_RTSP_HDR_USER_AGENT, sink->user_agent);

	gst_rtsp_message_add_header(&msg, GST_RTSP_HDR_CONTENT_TYPE, "application/sdp");

	// allocate sdp messege buffer... 
	res = gst_sdp_message_new(&sdp);

	//v=..
	res = gst_sdp_message_set_version(sdp, "0");
	//o=...
	res = gst_sdp_message_set_origin(sdp, "-","0", "0", "IN","IP4" , "0.0.0.0");

	//s=..
	if (sink->session_name)
		res = gst_sdp_message_set_session_name(sdp, "Unnamed");


	//i=..
	if (sink->information)
		res = gst_sdp_message_set_information(sdp, "N/A");


	//c=...
	res = gst_sdp_message_set_connection(sdp, "IN", "IP4", url_client_ip_str, 0, 0);
	//a=...
	res = gst_sdp_message_add_attribute(sdp, "recvonly", NULL);


	// create media
	GstSDPMedia *media;
	res = gst_sdp_media_new(&media);
	res = gst_sdp_media_init(media);

	//m=...
	res = gst_sdp_media_set_media(media, "video");
	
	res = gst_sdp_media_set_port_info(media, rtp_port, num_ports);
	res = gst_sdp_media_set_proto( media,"RTP/AVP");
	res = gst_sdp_media_add_format(media, szPayloadType);

	//a=...
	char *rtpmap = g_strdup_printf("%s H264/90000", szPayloadType);
	res = gst_sdp_media_add_attribute(media, "rtpmap", rtpmap);
	res = gst_sdp_media_add_attribute(media, "fmtp", szPayloadType);
	res = gst_sdp_media_add_attribute(media, "control", "streamid=0");



	// insert media into sdp
	res = gst_sdp_message_add_media(sdp,media);
	
	gchar * sdp_str = gst_sdp_message_as_text(sdp);
	size = g_utf8_strlen(sdp_str, 500);
	gst_sdp_message_free(sdp);
	gst_sdp_media_free(media);

	res = gst_rtsp_message_set_body(&msg, sdp_str, size);

	// Send our packet receive server answer and check some basic checks.
	if ((res = sendReceiveAndCheck(conn, &timeout, &msg, sink->debug)) != GST_RTSP_OK) {
		return res;
	}

	// get session number 
	szSessionNumber = extractSessionNumberFromMessage(&msg);



////////////////////// ANNOUNCE END //////////////////////////////////////////////////////////

	////////////////////// SETUP START //////////////////////////////////////////////////////////


	gint video_start_port = 5002;
	gint video_end_port = video_start_port + 1;
	gchar *transfer_foramt;
	gchar *tmp; 

	method = GST_RTSP_SETUP;
	tmp = g_strdup_printf("%s/streamid=0", url_server_str_full);
	res = gst_rtsp_message_init_request(&msg, method, tmp);
	if (res < 0)
		return res;

	transfer_foramt = g_strdup_printf("RTP/AVP/UDP;unicast;client_port=%d-%d;mode=record", video_start_port, video_end_port);
	
	gst_rtsp_message_add_header(&msg, GST_RTSP_HDR_TRANSPORT, transfer_foramt);
	gst_rtsp_message_add_header(&msg, GST_RTSP_HDR_SESSION, szSessionNumber); // TODO: Get the session id from the responce.

	gst_rtsp_message_add_header(&msg, GST_RTSP_HDR_CONTENT_LENGTH, "0"); // TODO: Get the session id from the responce.

	

	

	// Send our packet receive server answer and check some basic checks.
	if ((res = sendReceiveAndCheck(conn, &timeout, &msg, sink->debug)) != GST_RTSP_OK) {
		return res;
	}

	GstRTSPTransport *transport;
	res = gst_rtsp_transport_new(&transport);
	res = extractTransportFromMessage(&msg, transport);
	

	g_print("Got server port %d", transport->server_port);
	sink->server_rtp_port = transport->server_port.min;


	

	if (res != GST_RTSP_OK)
		return -ERR_PARSING; 

////////////////////// SETUP END //////////////////////////////////////////////////////////

	////////////////////// RECORD START //////////////////////////////////////////////////////////


	
	method = GST_RTSP_RECORD;
	res = gst_rtsp_message_init_request(&msg, method, url_server_str_full);
	if (res < 0)
		return res;

	
	gst_rtsp_message_add_header(&msg, GST_RTSP_HDR_RANGE, "npt=0.000-"); // start live.
	gst_rtsp_message_add_header(&msg, GST_RTSP_HDR_SESSION, szSessionNumber);


	// Send our packet receive server answer and check some basic checks.
	if ((res = sendReceiveAndCheck(conn, &timeout, &msg, sink->debug)) != GST_RTSP_OK) {
		return res;
	}

	////////////////////// RECORD END //////////////////////////////////////////////////////////



	//  if everything went OK lets setup UDP/RTP connection to server.
	res = setRTPConnectionToServer(sink);



	return GST_FLOW_OK;

beach:
	// free message and exit.
	return GST_FLOW_ERROR;

}


static gboolean default_unroll(GstBaseSink *media) {

	
	GstRTSPsink *sink = (GstRTSPsink*)media;
	GstRTSPConnection *conn = sink->conn;
	GstRTSPResult res;

	if (conn == NULL) {
		return GST_RTSP_OK;
	}

	// close connection 
	res = gst_rtsp_connection_close(conn);

	return res;
}


/* initialize the new element
 * instantiate pads and add them to element
 * set pad calback functions
 * initialize instance structure
 */
static void gst_rtspsink_init (GstRTSPsink * filter)
{
  
	
  filter->sinkpad = gst_pad_new_from_static_template (&sink_factory, "sink0");
  gst_pad_set_event_function (filter->sinkpad,    GST_DEBUG_FUNCPTR(gst_rtspsink_sink_event));
  //gst_pad_set_chain_function (filter->sinkpad,    GST_DEBUG_FUNCPTR(gst_rtspsink_chain));
  GST_PAD_SET_PROXY_CAPS (filter->sinkpad);
  gst_element_add_pad (GST_ELEMENT (filter), filter->sinkpad);

  filter->session_name = NULL;
  filter->information = NULL;

  

  filter->socket = NULL;

  filter->silent = FALSE;
}


static GstFlowReturn gst_rtsp_sink_render(GstBaseSink * bsink, GstBuffer * buffer)
{
	GstMapInfo map;
	GstRTSPsink *sink  = (GstRTSPsink*)bsink;

	// Let us access the data
	gst_buffer_map(buffer, &map, GST_MAP_READ);

	// send data over udp, period.
	if (g_socket_send_to(sink->socket, sink->sa, map.data, map.size, NULL, NULL) == -1 )
		g_print("Not godd sending failed !");

		
	if (sink->debug)
		g_print("Data len %d\n", map.size);
	
	gst_buffer_unmap(buffer, &map);

	return GST_FLOW_OK;
}

static void
gst_rtspsink_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstRTSPsink *filter = GST_RTSP_SINK (object);

  switch (prop_id) {
    case PROP_SILENT:
      filter->silent = g_value_get_boolean (value);
      break;
	case PROP_HOST:
		filter->host = g_strdup(g_value_get_string(value));
		break; 

	case PROP_STREAM_NAME:
		filter->stream_name = g_strdup(g_value_get_string(value));
		break;

	case PROP_PORT:
		filter->port = g_value_get_int(value);
		break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void gst_rtspsink_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  GstRTSPsink *filter = GST_RTSP_SINK (object);

  switch (prop_id) {
    case PROP_SILENT:
		g_value_set_boolean (value, filter->silent);
		break;
	case PROP_HOST:
		g_value_set_string(value, filter->host);
		break;

	case PROP_STREAM_NAME:
		g_value_set_string(value, filter->stream_name);
		break;

	case PROP_PORT:
		g_value_set_int(value, filter->port);
		break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

/* GstElement vmethod implementations */

/* this function handles sink events */
static gboolean
gst_rtspsink_sink_event (GstPad * pad, GstObject * parent, GstEvent * event)
{
  gboolean ret;
  GstRTSPsink *filter;

  filter = GST_RTSP_SINK (parent);

  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_CAPS:
    {
      GstCaps * caps;

      gst_event_parse_caps (event, &caps);
      /* do something with the caps */

      /* and forward */
      ret = gst_pad_event_default (pad, parent, event);
      break;
    }
    default:
      ret = gst_pad_event_default (pad, parent, event);
      break;
  }
  return ret;
}



/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
rtsp_sink_init (GstPlugin * rtsp_sink)
{
  /* debug category for fltering log messages
   *
   * exchange the string 'Template rtsp_sink' with your description
   */
  GST_DEBUG_CATEGORY_INIT (gst_rtspsink_debug, "rtsp_sink",  0, "Template rtsp_sink");

  return gst_element_register (rtsp_sink, "rtsp_sink", GST_RANK_NONE,  GST_TYPE_RTSP_SINK);
}

/* PACKAGE: this is usually set by autotools depending on some _INIT macro
 * in configure.ac and then written into and defined in config.h, but we can
 * just set it ourselves here in case someone doesn't use autotools to
 * compile this code. GST_PLUGIN_DEFINE needs PACKAGE to be defined.
 */
#ifndef PACKAGE
#define PACKAGE "myfirstrtsp_sink"
#endif


#define VERSION "0.0.1"


/* gstreamer looks for this structure to register rtsp_sinks
 *
 * exchange the string 'Template rtsp_sink' with your rtsp_sink description
 */
GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    rtsp_sink,
    "Pushing rtsp sink",
    rtsp_sink_init,
    VERSION,
    "LGPL",
    "GStreamer",
    "https://github.com/reporty/RTSPSink"
)
