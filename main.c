#include <stdio.h>
#include <stdlib.h>
#include <linux/hdreg.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/statvfs.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <stdint.h>
#include <unistd.h>
#include <dirent.h>

int main(int argc, char **argv)
{
	static struct hd_driveid hd;
	int fd;
	struct statvfs vfs;
	uint64_t file_size_in_bytes;
	struct hd_driveid hd_info;
	DIR *dp;
	struct dirent *ep;
	int mdma_modes;
	int pio_modes;
	int udma_modes;
	int ata_modes;


	if ((fd = open("/dev/sda", O_RDONLY | O_NONBLOCK)) < 0) {
		printf("ERROR opening /dev/sda\n");
		exit(1);
	}

	if (!ioctl(fd, HDIO_GET_IDENTITY, &hd)) {
		printf("Model: %.20s\n",  hd.model);
		printf("FwRev: %.8s\n", hd.fw_rev);
		printf("Serial number: %.15s\n", &hd.serial_no[5]);


		ioctl(fd, HDIO_GET_IDENTITY,&hd_info);

		pio_modes = hd_info.eide_pio_modes;
		if (pio_modes > 0) {
			printf("PIO modes: ");
			if(pio_modes & 1)
				printf("pio0 pio1 pio2 pio3 ");
			if(pio_modes & 2) {
				printf("pio4 ");
			}
			printf("\n");
		}


		mdma_modes = hd_info.dma_mword;
		if (mdma_modes > 0) {
			printf("DMA modes: ");

			if (mdma_modes & 1) {
				printf("mdma0 ");
			}
			if (mdma_modes & 2) {
				printf("mdma1 ");
			}
			if (mdma_modes & 4) {
				printf("mdma2 ");
			}
		}


		udma_modes = hd_info.dma_ultra;
		if (udma_modes & 1)
			printf("udma0 ");
		if (udma_modes & 2)
			printf("udma1 ");
		if (udma_modes & 4)
			printf("udma2 ");
		if (udma_modes & 8)
			printf("udma3 ");
		if (udma_modes & 16)
			printf("udma4 ");
		if (udma_modes & 32)
			printf("udma5 ");
		if (udma_modes & 64)
			printf("udma6 ");
		printf("\n");


		ata_modes = hd_info.major_rev_num;
		if (ata_modes > 0) {
			printf("ATA standarts: ");

			if (ata_modes & 32)
				printf("ATA/ATAPI-5 ");
			if (ata_modes & 64)
				printf("ATA/ATAPI-6 ");
			if (ata_modes & 128)
				printf("ATA/ATAPI-7 ");
			if (ata_modes & 256)
				printf("ATA8-ACS ");
			if (ata_modes & 512)
				printf("ACS-2 ");
			if (ata_modes & 1024)
				printf("ACS-3 ");
			printf("\n");
		}

		ioctl(fd, BLKGETSIZE64, &file_size_in_bytes);
		printf("Disk size: %.2f GiB\n", (double)file_size_in_bytes / (1024 * 1024 * 1024));

		close(fd);

		int partitions;
		if (partitions = open("/proc/partitions", O_RDONLY) < 0) {
			printf("ERROR opening /proc/partitions\n");
			exit(1);
		}

		if (fd = open("/boot", O_RDONLY | O_NONBLOCK)< 0) {
			printf("ERROR opening main.c\n");
			exit(1);
		}


	} else if (errno == -ENOMSG) {
		printf("No information in this field\n");
	} else {
		perror("ERROR: HDIO_GET_IDENTITY");
		close(fd);
		exit(1);
	}
	return 0;
}
