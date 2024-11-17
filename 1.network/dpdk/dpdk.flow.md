[TOC]
# dpdk flow rule
+ https://doc.dpdk.org/guides/testpmd_app_ug/testpmd_funcs.html#enqueueing-creation-of-flow-rules
    + 4.13.18.1. Attributes    = explain ingress, egress, transfer
    + 4.13.18.4. Pattern items = explain protocol member
    + 4.13.18.5. Actions list  = explain supported action

# flow rule syntax
```lua
-- Validate flow rule:
flow validate {port_id}
   [group {group_id}] [priority {level}]
   [ingress] [egress] [transfer]
   pattern {item} [/ {item} [...]] / end
   actions {action} [/ {action} [...]] / end

-- Create a flow rule:
flow create {port_id}
    [group {group_id}] [priority {level}]
    [ingress] [egress] [transfer]
    [tunnel_set {tunnel_id}] [tunnel_match {tunnel_id}] [user_id {user_id}]
    pattern {item} [/ {item} [...]] / end
    actions {action} [/ {action} [...]] / end

-- Destroy specific flow rules:
flow destroy {port_id} rule {rule_id} [...] [user_id]

-- Destroy all flow rules
flow flush {port_id}
```
## flow rule example
```
flow create 0 transfer pattern eth / ipv4 / udp / port_id id is 1 / end actions port_id id 0 / end
```