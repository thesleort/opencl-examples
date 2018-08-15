#get SDK prerequisites

YUM_CMD=$(which yum)
APT_GET_CMD=$(which apt-get)
#OTHER_CMD=$(which <other installer>)
$Packagename

if [[ ! -z $YUM_CMD ]]; then
	${Packagename:="yum"}
	wget --no-cookies --no-check-certificate --header "Cookie: gpw_e24=http%3A%2F%2Fwww.oracle.com%2F; oraclelicense=accept-securebackup-cookie" "http://download.oracle.com/otn-pub/java/jdk/8u60-b27/jdk-8u60-linux-x64.rpm"
    sudo yum localinstall jdk-8u60-linux-x64.rpm
	rm ~/jdk-8u60-linux-x64.rpm # remove the archived folder
	#Installing mono
	yum install yum-utils
	rpm --import "http://keyserver.ubuntu.com/pks/lookup?op=get&search=0x3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF"
    yum-config-manager --add-repo http://download.mono-project.com/repo/centos/
elif [[ ! -z $APT_GET_CMD ]]; then
	${Packagename:="apt-get"}
	apt-add-repository ppa:webupd8team/java
        apt-get update
        apt-get install oracle-java8-installer
	#mono install, from http://www.mono-project.com/docs/getting-started/install/linux/
	apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys     3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF
	echo "deb http://download.mono-project.com/repo/ubuntu stable-xenial main" | sudo tee /etc/apt/sources.list.d/mono-official-stable.list
	sudo apt-get update
else
	echo "error can't install package $PACKAGE"
exit 1;
fi
curl -O http://launchpadlibrarian.net/201330288/libicu52_52.1-8_amd64.deb
dpkg -i libicu52_52.1-8_amd64.deb
$Packagename update
$Packagename install mono-complete ca-certificates-mono
$Packagename install dkms
$Packagename install lsb-core