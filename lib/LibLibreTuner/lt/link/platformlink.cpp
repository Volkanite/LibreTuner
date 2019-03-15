#include "platformlink.h"

#include "../network/uds/isotpuds.h"
#include "../diagnostics/uds.h"
#include "../flash/mazdat1.h"

namespace lt {

network::CanPtr PlatformLink::can() {
	network::CanPtr can = datalink_.can(platform_.baudrate);
	if (!can) {
		throw std::runtime_error("CAN is unsupported with the selected datalink");
	}
	return can;
}

network::IsoTpPtr PlatformLink::isotp() {
	network::IsoTpPtr isotp = datalink_.isotp();
	if (!isotp) {
		isotp = std::make_unique<network::IsoTp>(can());
	}
	isotp->setOptions(network::IsoTpOptions{platform_.serverId, platform_.serverId + 8});
	return isotp;
}

network::UdsPtr PlatformLink::uds() {
	return std::make_unique<network::IsoTpUds>(isotp());
}

DtcScannerPtr PlatformLink::dtcScanner() {
	return std::make_unique<UdsDtcScanner>(uds());
}

FlasherPtr PlatformLink::flasher() {
    if (platform_.flashMode == "mazdat1") {
		return std::make_unique<MazdaT1Flasher>(uds(), FlashOptions{platform_.flashAuthOptions});
	}
    throw std::runtime_error("Invalid flash mode: " + platform_.flashMode);
}

} // namespace lt
