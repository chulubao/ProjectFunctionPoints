#Name: 软件包的名称，在后面的变量中即可使用%{name}的方式引用
Name: daemon 

#Version: 软件的实际版本号，例如：1.12.1等，后面可使用%{version}引用
Version: 0.0.1

#Release: 发布序列号，例如：1%{?dist}，标明第几次打包，后面可使用%{release}引用
Release: 1%{?dist}

#Summary: 软件包的内容
Summary: daemon program

#Group: 软件分组，建议使用：Applications/System
Group: Applications/System

#License: 软件授权方式GPLv2
License: GPLv2

#URL: 软件的URI
URL: https://www.baidu.com/

#作者(生产商)
Packager: clj <chulujian@126.com>

#Vendor: 打包组织或者人员(供应商)
Vendor: clj-firm

#Source: 源码包，可以带多个用Source1、Source2等源，后面也可以用%{source1}、%{source2}引用
#引用的源码文件
Source: %{name}-%{version}.tar.gz  
#引用配置文件
#Source1: nginx.conf  

#引用System-V风格的Service服务             
#Source2: nginx  

#引用日志轮转的配置文件                   
#Source3: nginx.logrotate           

#BuildRoot: 这个是安装或编译时使用的临时目录，即模拟安装完以后生成的文件目录：%_topdir/BUILDROOT 后面可使用$RPM_BUILD_ROOT 方式引用。
#虚拟根目录
BuildRoot:   %{_tmppath}/%{name}-%{version}-%{release}-root      
  

Prefix: 	/opt/daemonProgram

#所依赖的软件包
#Requires: libxslt-devel,openssl-devel,pcre-devel    



#%%description: 软件的详细说明
%description
user login system from clj

#%%define: 预定义的变量，例如定义日志路径: _logpath /var/log/weblog
#%%define:

#%%prep: 预备参数，通常为 %%setup -q（在安静模式下且最少输出）
%prep
%setup -q

#%%build: 编译参数 ./configure --user=nginx --group=nginx --prefix=/usr/local/nginx/……
#make后面的%{?_smp_mflags}意思是：如果是多处理器的话make时并行编译
%build
make %{?_smp_mflags}

#%%install:开始把软件安装到虚拟的根目录中(即$RPM_BUILD_ROOT）
#执行Makefile文件中make install
%install 
echo ${RPM_BUILD_ROOT}
make DESTDIR=$RPM_BUILD_ROOT install #安装到虚拟根
make DESTDIR=$RPM_BUILD_ROOT create #创建所需文件在虚拟根




#%%pre: 安装前需要做的任务，如：创建用户
%pre

#%%post: 安装后需要做的任务 如：自动启动的任务
%post

#向安全管理箱中添加图标
soft_config=/opt/software/config.ini
if [ -f $soft_config ]
then
	startLine=`sed -n '/deamonStart/=' $soft_config`
	lineAfter=5
	endLine=$(($startLine+$lineAfter))
	echo $endLine
	if [ $startLine > 0  ]
	then
		sed -i "${startLine},${endLine}d" $soft_config
	fi  
	echo "[daemon]" >> $soft_config
	echo "title=守护进程" >> $soft_config
	echo "cmd=/opt/daemonProgram/uiService" >> $soft_config
	echo "pixmap_path=/opt/daemonProgram/logo/logo.png" >> $soft_config
	echo "pixmap_path_ex=/opt/daemonProgram/logo/logo.png" >> $soft_config
	echo "status=0" >> $soft_config
fi

#启动后台进程程序
#chkconfig --add daemon :开机自启动daemon此shell脚本
if [ -f /etc/init.d/daemon ]
then
	chkconfig --add daemon
	if [ $? -ne 0 ]
	then
		exit 1
	else
	service  daemon start
	fi
fi

#if [ -f /opt/daemonProgram/DAEMON ];then
#	chmod	u+x  /opt/daemonProgram/DAEMON
#	./opt/daemonProgram/DAEMON
#fi



#%%preun: 卸载前需要做的任务 如：停止任务
%preun
if [ -f /etc/init.d/daemon ]     #暂停并删除服务
then
	service  daemon stop
	chkconfig --del daemon
fi


#%%postun: 卸载后需要做的任务 如：删除用户，删除/备份业务数据
#[0:卸载][1:安装][2:升级]
%postun
if [ ${1} == 0 ]; then                #//卸载后准备操作
	rm -rf  /opt/daemonProgram/
	rm -rf  /etc/init.d/daemon
	rm -rf  /var/lock/daemon_singleton 
	
	soft_config=/opt/software/config.ini
	if [ -f $soft_config  ];then
		startLine=`sed -n '/deamonEnd/=' $soft_config`
		lineAfter=5
		endLine=$(($startLine+$lineAfter))
		echo $endLine
		if [ $startLine > 0  ];then
			sed -i "${startLine},${endLine}d" $soft_config
		fi  
	fi
fi


#%%clean: 清除上次编译生成的临时文件，就是上文提到的虚拟目录
%clean
#rm -rf %%{buildroot}



#%%files:
#【1】本段是文件段，用于定义构成软件包的文件列表，那些文件或目录会放入rpm中，分为三类-说明文档（doc），配置文件（config）及执行程序。（定义rpm包安装时创建的相关目录及文件）
#【2】设置文件属性，定义文件存取权限，拥有者及组别。
#【3】%%file部分使用则是相对路径。
#【4】%%defattr (-,root,root)：指定安装文件的属性，分别是(mode,owner,group)，-表示默认值，对文本文件是0644，可执行文件是0755。
#【5】%%defattr (-,root,root,0777)：指定安装文件的属性，分别是(mode,owner,group)，-表示默认值，对文本文件是0644，可执行文件是0755,权限模式为umask定义（也可以用0777等模式值显式指定）。
#【6】fattr(-,root,root) #设置文件的默认权限,-表示默认值，对文本文件是0644，可执行文件是0755。
#【7】%%attr(mode, user, group) filename #控制文件的权限如%attr(0644,root,root) /etc/yp.conf
%files
%defattr(-,root,root,0777)
/opt/daemonProgram/localSocket/
/opt/daemonProgram/uiService
/opt/daemonProgram/DAEMON
/usr/share/applications/start.desktop
/opt/daemonProgram/logo/logo.png
/etc/init.d/daemon
/opt/daemonProgram/logDebug/
#%%dir /var/log/loginSystemLog
#%%attr(644,root,root) %{_prefix}/conf/nginx.conf
#%%attr(755,root,root) /etc/rc.d/init.d/nginx


#%%changelog: 修改历史
%changelog
#*Mon Feb 18 2019 Nme1
#*Added missing requires
#*Tue Feb 18 2019 Nme2
#*delete missing requires
	



