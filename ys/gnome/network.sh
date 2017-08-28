#!/bin/bash
programname=$0
function usage {
    echo "usage: $programname [-i in] [-o out]"
    echo "*  -i  internal interface"
    echo "*  -o  external interface"
    exit 1
}
if [ "$#" -ne 4 ] ; then
    usage
fi
if [ "$(ping -c 5 -w 5 8.8.8.8 | grep icmp_seq)" == "" ] ; then
    echo Check network. 
    exit 1
fi
while getopts "i:o:" op 
do
   case "${op}" in
        i)
                internal=${OPTARG}
                checkinternal=$(ip addr | grep $internal)
                if [ "$checkinternal" != "" ]
                 then
                        if [ "$(rpm -qa | grep iptables-services)" == "" ] ; then
                                yum install -y iptables-services > /dev/null 2>&1
                        fi
                        if [ "$(sed  -n "/IPADDR=.*/p" /etc/sysconfig/network-scripts/ifcfg-$internal)" == "" ]
                         then
                                sed -i "s/IPADDR=.*/IPADDR=10.1.1.1/g" /etc/sysconfig/network-scripts/ifcfg-$internal
                         else
                                echo "IPADDR=10.1.1.1" >> /etc/sysconfig/network-scripts/ifcfg-$internal
                        fi
                        sed -i "s/BOOTPROTO=.*/BOOTPROTO=none/g" /etc/sysconfig/network-scripts/ifcfg-$internal
                        sed -i "s/ONBOOT=.*/ONBOOT=yes/g" /etc/sysconfig/network-scripts/ifcfg-$internal
                 else
                        echo "\n\nDevice "$internal" does not exist.\n\n"                       
                        usage
                fi

           ;;
        o)
                external=${OPTARG}
                checkexternal=$(ip addr | grep $external)
                if [ "$checkexternal" != "" ]
                 then
                        iptables -F FORWARD
                        iptables -P FORWARD DROP
                        echo net.ipv4.ip_forward = 1 > /etc/sysctl.conf
                        echo 1 > /proc/sys/net/ipv4/ip_forward
                        /sbin/sysctl -p
                        iptables -A FORWARD -o $internal -j ACCEPT
                        iptables -A FORWARD -o $external -j ACCEPT
                        iptables -t nat -A POSTROUTING -o $external -j MASQUERADE
                        service iptables save
                        systemctl restart network
                        echo -e "\n\n IPADDR=10.1.1.2 ~ 10.1.1.255, GATEWAY=10.1.1.1 \n\n"
                 else
                        echo "\n\nDevice "$external" does not exist.\n\n"
                        usage
                fi
           ;;
        ?)
                usage
           ;;
   esac
done
shift $(( $OPTIND - 1 ))

                        
