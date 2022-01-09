#include "ide_pci.h"

bool ide_pci_init(){
	channel_regs channels[2]; //Temporary. Array of channels on a single controller
	ide_device devices[4]; //Temporary. Array of all devices on a single controller

    for(int i = 0; i<pci_get_number_of_devices(); i++){
        pci_device* dev = pci_get_device(i);
		char buff[64];
        if(dev->class_code == 0x1 && dev->subclass == 0x1){
            logger_log_info("IDE Controller found!");
            logger_log_info(itoa(dev->vendor_id, buff, 16));
            logger_log_info(itoa(dev->header_type, buff, 16));
            logger_log_info(itoa(dev->class_code, buff, 16));
            logger_log_info(itoa(dev->subclass, buff, 16));
            logger_log_info(itoa(dev->prog_if, buff, 16));

            uint8_t status = dev->prog_if;
            if(status & 0x1)
                logger_log_info("Channel primary in native-PCI mode");
            else
                logger_log_info("Channel primary in compability mode");
            if(status & 0x2)
                logger_log_info("Channel primary can have it's operation mode changed");
            else
                logger_log_info("Channel primary has fixed operation mode");
            if(status & 0x4)
                logger_log_info("Channel secondary in native-PCI mode");
            else
                logger_log_info("Channel secondary in compability mode");
            if(status & 0x8)
                logger_log_info("Channel secondary can have it's operation mode changed");
            else
                logger_log_info("Channel secondary has fixed operation mode");
        
			//Read address from base register, or use default values
			channels[ATA_PRIMARY].base = (dev->base_addres_0 & 0xFFFFFFFC) + 0x1F0 * (!dev->base_addres_0);
			channels[ATA_PRIMARY].control = (dev->base_addres_1 & 0xFFFFFFFC) + 0x3F6 * (!dev->base_addres_1);
			channels[ATA_SECONDARY].base = (dev->base_addres_2 & 0xFFFFFFFC) + 0x170 * (!dev->base_addres_2);
			channels[ATA_SECONDARY].control = (dev->base_addres_3 & 0xFFFFFFFC) + 0x376 * (!dev->base_addres_3);
			channels[ATA_PRIMARY].master = (dev->base_addres_4 & 0xFFFFFFFC);
			channels[ATA_SECONDARY].master = (dev->base_addres_4 & 0xFFFFFFFC) + 8;

			ide_write(&channels[ATA_PRIMARY]  , ATA_REG_CONTROL, 2);
			ide_write(&channels[ATA_SECONDARY], ATA_REG_CONTROL, 2);
			for (int j = 0; j < 2; j++){
				for (int k = 0; k < 2; k++){
					bool err = false;
					uint8_t buffer[512];
					ide_write(&channels[j], ATA_REG_HDDEVSEL, 0xA0 | k << 4);
					sleep(10);
					ide_write(&channels[j], ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
					sleep(10);

					uint8_t tmp =ide_read(&channels[j], ATA_REG_STATUS); 
					logger_log_info(itoa(tmp, buff, 16));
					if( tmp == 0) 
						logger_log_info("NO DRIVE!");
					else{
						logger_log_info("Something detected");
						while(1) {
							status = ide_read(&channels[j], ATA_REG_STATUS);
							if ((status & ATA_SR_ERR)) {
								logger_log_info("Error while identifying drive!");
								err = true;
								break;
							} // If Err, Device is not ATA.
							if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ)){
								logger_log_info("Ready to read!");

								break; // Everything is right.
							}
						}

						if(!err){
							for(int l = 0; l < 512; l++){
								buffer[l] = ide_read(&channels[j], ATA_REG_DATA);
							}
						}
					}
				}
			}


            
        }
    }
	return false;
}

void ide_write(channel_regs* channel, uint8_t reg, uint8_t data){
	if (reg > 0x07 && reg < 0x0C)
		ide_write(channel, ATA_REG_CONTROL, 0x80 | channel->ni);
	if (reg < 0x08)
		io_out_byte(channel->base  + reg - 0x00, data);
	else if (reg < 0x0C)
		io_out_byte(channel->base  + reg - 0x06, data);
	else if (reg < 0x0E)
		io_out_byte(channel->control  + reg - 0x0A, data);
	else if (reg < 0x16)
		io_out_byte(channel->master + reg - 0x0E, data);
	if (reg > 0x07 && reg < 0x0C)
		ide_write(channel, ATA_REG_CONTROL, channel->ni);
}

uint8_t ide_read(channel_regs* channel, uint8_t reg){
	unsigned char result;
	if (reg > 0x07 && reg < 0x0C)
		ide_write(channel, ATA_REG_CONTROL, 0x80 | channel->ni);
	if (reg < 0x08)
		result = io_in_byte(channel->base + reg - 0x00);
	else if (reg < 0x0C)
		result = io_in_byte(channel->base  + reg - 0x06);
	else if (reg < 0x0E)
		result = io_in_byte(channel->control  + reg - 0x0A);
	else if (reg < 0x16)
		result = io_in_byte(channel->master + reg - 0x0E);
	if (reg > 0x07 && reg < 0x0C)
		ide_write(channel, ATA_REG_CONTROL, channel->ni);
	return result;
}