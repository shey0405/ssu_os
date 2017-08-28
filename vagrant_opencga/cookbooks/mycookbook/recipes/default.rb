#
# Cookbook Name:: mycookbook
# Recipe:: default
#
# Copyright (C) 2017 YOUR_NAME
#
# All rights reserved - Do Not Redistribute
#



bash "seting" do
  code <<-EOH
  yum -y install git
  yum -y update
  yum -y install curl
  yum -y update
  yum -y install vim
  yum -y install curl
  EOH
end

bash 'arangodb' do
  code <<-EOH
  cd /etc/yum.repos.d/
  curl -O https://www.arangodb.com/repositories/arangodb31/CentOS_7/arangodb.repo
  yum -y install arangodb3
  sudo sed -i "s/authentication = true/authentication = false/g" /etc/arangodb3/arangod.conf
  sudo systemctl restart arangodb3
  EOH
end


bash 'java_install' do
  user 'root'
  group 'root'
  cwd '/opt'
  code <<-EOH
  cd /opt
  sudo wget --no-cookies --no-check-certificate --header "Cookie: oraclelicense=accept-securebackup-cookie" -P /opt/ "http://download.oracle.com/otn-pub/java/jdk/8u144-b01/090f390dda5b47b9b721c7dfaa008135/jdk-8u144-linux-x64.rpm"
  rpm -ivh jdk-8u144-linux-x64.rpm
  EOH
end

bash 'maven1' do
  code <<-EOH
  cd /opt
  wget http://www-eu.apache.org/dist/maven/maven-3/3.5.0/binaries/apache-maven-3.5.0-bin.tar.gz
  tar -zxvf apache-maven-3.5.0-bin.tar.gz
  ln -s -f apache-maven-3.5.0 maven
  EOH
end

file "maven.sh" do
  path "/etc/profile.d/maven.sh"
  content "export M2_HOME=/opt/maven\nexport PATH=${M2_HOME}/bin:${PATH}"
  owner "root"
  group "root"
  mode 0777
end

bash 'maven2' do
  cwd '/opt'
  code <<-EOH
  source /etc/profile.d/maven.sh
  EOH
end



file "mongodb-org-3.4.repo" do
  path "/etc/yum.repos.d/mongodb-org-3.4.repo"
  content "[mongodb-org-3.4]\nname=MongoDB Repository\nbaseurl=https://repo.mongodb.org/yum/redhat/$releasever/mongodb-org/3.4/x86_64/\ngpgcheck=1\nenabled=1\ngpgkey=https://www.mongodb.org/static/pgp/server-3.4.asc"
  owner "root"
  group "root"
  mode 0777
end

bash 'mongo' do
  cwd '/opt'
  code <<-EOH
  yum install -y mongodb-org.x86_64
  systemctl start mongod
  EOH
end


bash 'tomcat' do
  cwd '/opt'
  user 'root'
  group 'root'
  code <<-EOH
  wget http://mirror.nexcess.net/apache/tomcat/tomcat-8/v8.5.20/bin/apache-tomcat-8.5.20.tar.gz
  tar -zxvf apache-tomcat-8.5.20.tar.gz
  EOH
end



bash 'opencga1' do
  user 'root'
  group 'root'
  cwd '/opt'
  code <<-EOH
  git clone https://github.com/opencb/opencga.git
  EOH
end

bash 'opencga2' do
  user 'root'
  group 'root'
  cwd '/opt/opencga'
  code <<-EOH
  git checkout v1.0.0
  EOH
end


bash 'cellbase' do
  user 'root'
  group 'root'
  cwd '/opt'
  code <<-EOH
  git clone -b release-4.5.0-rc.1 https://github.com/opencb/cellbase.git
  EOH
end

file '/opt/opencga_pom.sh' do
  user 'root'
  group 'root'
  mode 0777
  content "#!/bin/bash
  sudo sed -i s/opt\\\\/opencga/opt\\\\/opencga\\\\/build/ /opt/opencga/pom.xml
  sudo sed -i s/v4/v3/ /opt/opencga/pom.xml
  sudo sed -i s/bioinfo.hpc.cam.ac.uk/localhost:8080/ /opt/opencga/pom.xml
  sudo sed -i '$ a export JAVA_HOME=/usr/java/jdk1.8.0_144' /etc/profile
  sudo sed -i '$ a export JRE_HOME=/usr/java/jdk1.8.0_144/jre' /etc/profile
  sudo sed -i '$ a PATH=$PATH:$HOME/bin:/usr/java/jdk1.8.0_144/bin' /etc/profile
  sudo sed -i '/<profiles>$/a <profile>\n<id>localhost</id>\n<activation>\n<activeByDefault>true</activeByDefault>\n</activation>\n<properties>\n<CELLBASE.DB.HOST>localhost:27017</CELLBASE.DB.HOST>\n<CELLBASE.DB.PORT>27017</CELLBASE.DB.PORT>\n<CELLBASE.DB.USER></CELLBASE.DB.USER>\n<CELLBASE.DB.PASSWORD></CELLBASE.DB.PASSWORD>\n<CELLBASE.DB.MONGODB.AUTHENTICATIONDATABASE></CELLBASE.DB.MONGODB.AUTHENTICATIONDATABASE>\n<CELLBASE.DB.MONGODB.READPREFERENCE>nearest</CELLBASE.DB.MONGODB.READPREFERENCE>\n<CELLBASE.VERSION>v3</CELLBASE.VERSION>\n<CELLBASE.ENSEMBL.LIBS>/home/imedina/apis/ensembl/api_79</CELLBASE.ENSEMBL.LIBS>\n<CELLBASE.DB.MONGODB.REPLICASET></CELLBASE.DB.MONGODB.REPLICASET>\n</properties>\n</profile>' /opt/cellbase/pom.xml
  source /etc/profile"
end

bash 'run' do
  user 'root'
  group 'root'
  cwd '/opt'
  code <<-EOH
  /opt/opencga_pom.sh
  EOH
end
