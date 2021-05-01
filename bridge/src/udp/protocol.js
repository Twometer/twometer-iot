const PacketType = {
    Discovery: 'DISCOV',
    BridgeHello: 'BHELLO',
    DeviceHello: 'DHELLO',
    UpdateProperty: 'UPDATE',
    ReportChange: 'REPORT',
    Ping: 'PING',
    Pong: 'PONG'
}

function parseMessage(buffer) {
    let rawMessage = buffer.toString('utf-8');
    if (!rawMessage.includes(':')) { // Invalid message
        return null;
    }

    let messageParts = rawMessage.split(':');
    return {type: messageParts[0], params: messageParts.splice(1)}
}

function createMessage(type, ...params) {
    return `${type}:${params.join(':')}`;
}

module.exports = {MsgType: PacketType, parseMessage, createMessage}