#pragma once

#include <map>
#include <mutex>
#include <vector>

#include "connection.h"
#include "device.h"
#include "dronecore.h"
#include "mavlink_include.h"

namespace dronecore {

class DroneCoreImpl
{
public:
    DroneCoreImpl();
    ~DroneCoreImpl();

    void receive_message(const mavlink_message_t &message);
    bool send_message(const mavlink_message_t &message);
    ConnectionResult add_any_connection(const std::string &connection_url);
    ConnectionResult add_link_connection(const std::string &protocol, const std::string &ip,
                                         int port);
    ConnectionResult add_udp_connection(int local_port_number);
    void add_connection(Connection *connection);
    ConnectionResult add_tcp_connection(const std::string &remote_ip, int remote_port);
    ConnectionResult add_serial_connection(const std::string &dev_path, int baudrate);

    const std::vector<uint64_t> &get_device_uuids() const;
    Device &get_autopilot();
    Device &get_autopilot(uint64_t uuid);

    bool is_autopilot_connected() const;
#if 0
    bool is_connected(uint64_t uuid) const;
#endif

    void register_on_discover(DroneCore::event_callback_t callback);
    void register_on_timeout(DroneCore::event_callback_t callback);

    void notify_on_discover(uint64_t uuid, uint8_t component_id);
    void notify_on_timeout(uint64_t uuid, uint8_t component_id);

private:
    bool make_new_device(uint8_t ystem_id, uint8_t comp_id);

    std::mutex _connections_mutex;
    std::vector<Connection *> _connections;

    mutable std::recursive_mutex _devices_mutex;
    std::map<uint8_t, std::vector<Device *>> _devices;

    DroneCore::event_callback_t _on_discover_callback;
    DroneCore::event_callback_t _on_timeout_callback;

    std::atomic<bool> _should_exit = {false};
};

} // namespace dronecore
