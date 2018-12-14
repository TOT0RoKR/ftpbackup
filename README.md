# FTP 자동 백업

### Concept

#### ftpautoupload.c

main의 file_path 에 명시된 디렉토리의 하위 경로에
수정시간이 변화하면(파일이 변경되면) file_path에 해당하는
디렉토리를 tar로 묶은 뒤 ftp서버로 전송.
<br/>
파일 수정시간을 string으로 변환후 hash함수를 이용해 모든
파일의 수정시간을 XOR 연산하여 변화가 발생하면, 파일이 변경된
것으로 확인.

#### ftpautoversion.c

ftp 디렉토리에 autoupload로 받은 tar 파일을 지속적으로 검사하고
해당 파일이 생성되면 현재 시간에 해당하는 디렉토리를 생성하여
위치를 옮기고 log를 남김.
