@echo off
 
echo app.keystoreファイルを生成します
 
if exist app.keystore rm app.keystore
 
"%JAVA_HOME%\bin\keytool" -v -genkey -keystore .\app.keystore -validity 10000 -dname "cn=Denta Torai, ou=GameScience, o=TridentComputer, c=JP" -keypass password -storepass password
 
echo 完了！
 
pause