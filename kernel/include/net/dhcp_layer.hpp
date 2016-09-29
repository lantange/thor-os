//=======================================================================
// Copyright Baptiste Wicht 2013-2016.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://www.opensource.org/licenses/MIT)
//=======================================================================

#ifndef NET_DHCP_LAYER_H
#define NET_DHCP_LAYER_H

#include <types.hpp>
#include <expected.hpp>
#include <atomic.hpp>

#include "net/interface.hpp"
#include "net/packet.hpp"

namespace network {

namespace udp {
struct layer;
}

namespace dhcp {

/*!
 * \brief The DHCP packet header
 */
struct header {
    uint8_t op;                 ///< Operation code
    uint8_t htype;              ///< Hardware type
    uint8_t hlen;               ///< Hardware Address Length
    uint8_t hops;               ///< Control forwarding to DHCP relays
    uint32_t xid;               ///< Identification generated by the client
    uint16_t secs;              ///< Number of seconds elapsed since beginning of attempt
    uint16_t flags;             ///< Flags
    uint32_t client_ip;         ///< Client IP Address
    uint32_t your_ip;           ///< IP Address that the server is assigning to the client
    uint32_t server_ip;         ///< Server's IP address to use for the next step
    uint32_t gw_ip;             ///< Gateways's IP address for DHCP relays
    uint8_t client_haddr[16];   ///< Client hardware address
    uint8_t server_name[64];    ///< Server name
    uint8_t boot_filename[128]; ///< Boot file name requested
};

/*!
 * \brief The received DHCP configuration
 */
struct dhcp_configuration {
    network::ip::address ip_address;      ///< The IP address
    network::ip::address dns_address;     ///< The DNS address
    network::ip::address gateway_address; ///< The gateway address

    bool dns     = false; ///< Indicates if there is a DNS
    bool gateway = false; ///< Indicates if there is a gateway
};

struct layer {
    layer(network::udp::layer* parent);

    /*!
     * \brief Decode a network packet.
     *
     * This must only be called from the UDP layer.
     *
     * \param interface The interface on which the packet was received
     * \param packet The packet to decode
     */
    void decode(network::interface_descriptor& interface, network::packet& packet);

    /*!
     * \brief Request an IP address on the network
     *
     * \param interface The interface for which we want an IP address
     */
    std::expected<dhcp_configuration> request_ip(network::interface_descriptor& interface);
private:
    network::udp::layer* parent;

    std::atomic<bool> listening;
    circular_buffer<network::packet, 16> packets;
    condition_variable listen_queue;
};

} // end of dns namespace

} // end of network namespace

#endif
