MAIN_DIR = "daemon/"
UI_DIR = "ui/"

.PHONY:  main  qtui 

all:main  qtui 

main:
	(make -C $(MAIN_DIR)) 

qtui:
	(make -C $(UI_DIR))

install :
	@echo "=================make install================"
	test -d $(DESTDIR)/opt/daemonProgram/ || install -d $(DESTDIR)/opt/daemonProgram/
	install -m 755  daemon/DAEMON   $(DESTDIR)/opt/daemonProgram/
	install -m 755  ui/uiProcess/uiService    $(DESTDIR)/opt/daemonProgram/
	
	test -d $(DESTDIR)/user/share/applications/ || install -d $(DESTDIR)/usr/share/applications/
	install -m 755  logo/start.desktop $(DESTDIR)/usr/share/applications/
	
	test -d $(DESTDIR)/opt/daemonProgram/logo/ || install -d $(DESTDIR)/opt/daemonProgram/logo/
	install -m 755  logo/logo.png   $(DESTDIR)/opt/daemonProgram/logo/
	
	test -d $(DESTDIR)/etc/init.d/ || install -d $(DESTDIR)/etc/init.d/
	install -m 755  daemon/daemon    $(DESTDIR)/etc/init.d/

create:	
	@echo "=================make create================"
	test -d $(DESTDIR)/opt/daemonProgram/localSocket/  || install -d $(DESTDIR)/opt/daemonProgram/localSocket/
	touch  $(DESTDIR)/opt/daemonProgram/localSocket/local_socket.account
	chmod 666 $(DESTDIR)/opt/daemonProgram/localSocket/local_socket.account
	
	test -d $(DESTDIR)/opt/daemonProgram/logDebug/  || install -d $(DESTDIR)/opt/daemonProgram/logDebug/
	touch  $(DESTDIR)/opt/daemonProgram/logDebug/ui.log
	chmod 666 $(DESTDIR)/opt/daemonProgram/logDebug/ui.log
	touch  $(DESTDIR)/opt/daemonProgram/logDebug/communication.log
	chmod 666 $(DESTDIR)/opt/daemonProgram/logDebug/communication.log


clean:
	make clean -C $(MAIN_DIR)
	make clean -C $(UI_DIR)

