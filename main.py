import re
import os

partition_list = list()
mountpoints_list = list()
free_space = 0

def get_partition_list():
    partitions = open("/proc/partitions")
    string_list = partitions.readlines()
    for string in string_list:
        temp = string.split()
        if re.match(r'.*sda[0-9]\b', string):
            partition_list.append('/dev/' + temp[-1:][0])
    partitions.close()


def get_mountpoints():
    mtab = open("/etc/mtab")
    string_list = mtab.readlines()
    for string in string_list:
        temp = string.split()
        for partition in partition_list:
            if string.find(partition) == 0:
                mountpoints_list.append(temp[1])
    mtab.close()


def get_free_space():
    global free_space
    get_partition_list()
    get_mountpoints()
    for mountpoint in mountpoints_list:
        st = os.statvfs(mountpoint)
        free = st.f_bavail * st.f_bsize
        free_space += free / (1024*1024*1024)


def get_hd_info():
    get_free_space()
    os.system("./main > temp")
    info = open("temp")
    string_list = info.readlines()
    hd_size_string = string_list[-1:][0].split()
    hd_size = float(hd_size_string[2])
    for string in string_list:
        print(string[:-1])
    print('Free: %.2f' % free_space + ' GiB')
    used = float(hd_size - free_space)
    print('Used: %.2f' % used + ' GiB')
    info.close()


get_hd_info()
