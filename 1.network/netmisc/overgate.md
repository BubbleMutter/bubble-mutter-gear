[TOC]
# over gate
# cloud server provider tips
+ vultr: never work any more.
+ digital ocean: create and destroy until you figure out a working one
    + easily ban your account
+ bandwagom: more and more ip get banned
    + unable to delete account (Canadian Laws)
+ hostwinds: pay methods are frustrating
+ aws cloud:

# caddy vmess tls wss cdn
1. caddy
```js
{
    acme_dns cloudflare $api_token
}

$domain {
    root * /var/www
    file_server browse
    log {
        output file /var/log/caddy/caddy.log
    }
    tls {
        dns cloudflare $api_token
        resolvers 8.8.8.8
    }
    @websocket {
        path /cf365c6b664b
        header Connection *Upgrade*
        header Upgrade websocket
    }
    reverse_proxy @websocket 127.0.0.1:1080
}
```

2. server
```json
{
  "inbounds": [
    {
      "port": 1080,
      "listen":"127.0.0.1",
      "protocol": "vmess",
      "settings": {
        "clients": [
          {
            "id": "$uuid",
            "alterId": 64
          }
        ]
      },
      "streamSettings": {
        "network": "ws",
        "wsSettings": {
            "path": "/$wsspath"
        }
      }
    }
  ],
  "outbounds": [
    {
      "protocol": "freedom",
      "settings": {}
    }
  ]
}
```

3. client
```json
{
    "log": {
        "loglevel":"info"
    },
    "inbounds": [{
        "port": 1080,
        "listen": "127.0.0.1",
        "protocol": "socks",
        "sniffing": {
            "enabled": false,
            "destOverride": ["http", "tls"]
        },
        "settings": {
            "auth": "noauth",
            "udp": false
        }
    },{ "port": 1088,
        "listen": "127.0.0.1",
        "protocol": "http",
        "sniffing": {
            "enabled": false,
            "destOverride": ["http", "tls"]
        },
        "settings": {
            "auth": "noauth",
            "udp": true
        }
    }],
    "outbounds": [{
        "protocol": "vmess",
        "settings": {
            "vnext":[{
                "address": "$domain",
                "port":443,
                "users": [{
                    "id": "$uuid",
                    "alterId": 64
                }]
            }]
        },
      "streamSettings": {
        "network": "ws",
        "security": "tls",
        "tlsSettings": {
            "allowInsecure": false
        },
        "wsSettings": { "path": "/$wsspath" }
      }
    },
    {
        "protocol": "freedom",
        "settings": {},
        "tag": "direct"
    }],
    "routing": {
        "domainStrategy": "IPOnDemand",
        "rules": [{
            "type": "field",
            "outboundTag": "direct",
            "ip": [ "geoip:cn", "geoip:private" ]
        }]
    }
}
```
4. DNS Provider
    1. Modify dns server list, add cloudflare
5. cloudflare
    1. Set Ssl Mode = Full
    2. Add DNS Record = root and www
    3. Set Redirect HTTP to HTTPS
    4. Enable Web Socket
    5. Wait **Edge Certificates** to become Active

