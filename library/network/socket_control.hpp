#ifndef INCLUDED__cx__network__socket_control__hpp
#define INCLUDED__cx__network__socket_control__hpp





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool set_socket_option_multicast_ttl   (socket_descriptor_t descriptor, std::uint8_t multicast_ttl=64u);
bool set_socket_option_multicast_loop  (socket_descriptor_t descriptor, bool multicast_loop=true);
/*
bool set_socket_option_multicast_if    (socket_descriptor_t descriptor, struct in_addr& address);
*/
bool set_socket_option_multicast_if    (socket_descriptor_t descriptor, const char* _local);




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*
bool set_socket_option_add_membership  (socket_descriptor_t descriptor, struct ip_mreq& address);
bool set_socket_option_drop_membership (socket_descriptor_t descriptor, struct ip_mreq& address);
*/
bool set_socket_option_add_membership  (socket_descriptor_t descriptor, const char* _local, const char* _group);
bool set_socket_option_drop_membership (socket_descriptor_t descriptor, const char* _local, const char* _group);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool set_socket_option_broadcast   (socket_descriptor_t descriptor, bool enable_broadcast=true);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool set_socket_option_keepalive   (socket_descriptor_t descriptor, bool keepalive=true);
bool set_socket_option_reuseaddr   (socket_descriptor_t descriptor, bool reuseaddr=true);
bool set_socket_option_tcp_nodelay (socket_descriptor_t descriptor, bool tcp_nodelay=true);
bool set_socket_option_linger      (socket_descriptor_t descriptor, bool onoff=true, std::uint16_t timeout=0u);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool set_socket_io_control_none_blocking_mode (socket_descriptor_t descriptor, bool enable=true);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool set_socket_option_tx_timeout(socket_descriptor_t descriptor, std::uint32_t msec);
bool set_socket_option_rx_timeout(socket_descriptor_t descriptor, std::uint32_t msec);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif

