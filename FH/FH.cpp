#include <iostream>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <time.h>
using namespace std;

int FileNameCount=1;//文件存储下标计数器 
char DataBuff[25500];//文件数据缓存区 
char NameBuff[255];//文件命名缓存区 
char AddressBuff[255];//文件地址缓存区 
void ShowSize(HWND);//展示句柄尺寸大小 

typedef struct Name{
	string file_address;//文件所在的地址 
	string file_name;//用户命名的文件名 
}FileData;
vector<FileData> FileRoom;//文件存储列表 
vector<HWND> TargetHandleRoom;//目标句柄栈 

BOOL CALLBACK EnumWindowsProc(HWND, LPARAM);//找到所有主窗口句柄 
BOOL CALLBACK EnumChildProc(_In_ HWND, _In_ LPARAM);//找到主窗口下子窗口的控件、窗格等 
void ShowSize(HWND);//展示句柄窗口的尺寸大小
void ShowFileContent(FileData);//显示文件内容
void BuildTextFile();//构建新文件保存至本地
void OutputTextFileContentTo_FABU(FileData,HWND);//将文件内容输出至发布中 
void ShowAllCurrentFiles();//显示当前已构建的所有文件
int SelectTargetFile();//获取指定的文件在文件列表中的下标 
HWND SearchSelectedHandle();//获取目标句柄 
void ShowMenuInterface();//显示菜单界面
void ShowTipsInterface();//显示提示界面 
void ProgramEncryption();//程序伪加密(测试使用) 

BOOL CALLBACK EnumChildProc(_In_ HWND   hwnd, _In_ LPARAM lParam){
    char szTitle[MAX_PATH] = {0};
    char szClass[MAX_PATH] = {0};
    int nMaxCount = MAX_PATH;

    LPSTR lpClassName = szClass;
    LPSTR lpWindowName = szTitle;

    GetWindowTextA(hwnd, lpWindowName, nMaxCount);
    GetClassNameA(hwnd, lpClassName, nMaxCount);
//    cout << "[Child window] window handle: " << hwnd << " window name: " 
//        << lpWindowName << " class name " << lpClassName << endl; 
    
    RECT rect;  
	GetWindowRect(hwnd,&rect); 	
	int w=rect.right-rect.left,h=rect.bottom-rect.top;
	
	if(strcmp(lpClassName,"TRichEdit")==0&&(w!=685&&h!=89)&&(w!=137&&h!=105)){
		//ShowSize(hwnd);
		TargetHandleRoom.push_back(hwnd);
	}
    return TRUE;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam){
    char szTitle[MAX_PATH] = {0};
    char szClass[MAX_PATH] = {0};
    int nMaxCount = MAX_PATH;

    LPSTR lpClassName = szClass;
    LPSTR lpWindowName = szTitle;

    GetWindowTextA(hwnd, lpWindowName, nMaxCount);
    GetClassNameA(hwnd, lpClassName, nMaxCount);
    
	string TempString=lpClassName;
	if(TempString!="TSDIAppForm"){
		return TRUE;
	}
	
//    cout << "[Parent window] window handle: " << hwnd << " window name: " 
//        << lpWindowName << " class name " << lpClassName << endl; 

    EnumChildWindows(hwnd, EnumChildProc, lParam);

    return TRUE;
}

void ShowSize(HWND hq){
	RECT rect;  
	GetWindowRect(hq,&rect); 	
	int w=rect.right-rect.left,h=rect.bottom-rect.top;
	cout<<"handle: "<<hq<<"    宽:"<<w<<" "<<"高:"<<h<<endl;
}

void ShowFileContent(FileData FD){
	//输出数据 
	memset(DataBuff,0,sizeof(DataBuff));
	memset(AddressBuff,0,sizeof(AddressBuff));
	strcpy(AddressBuff,FD.file_address.c_str());
    int cnt=0;
    char ch;
    FILE *fp=fopen(AddressBuff,"r");
    if(fp==NULL){
		cout<<"程序出现异常,将关闭程序"<<endl;
		system("exit");
	}
	
    while((ch=fgetc(fp))!=EOF)      
        DataBuff[cnt++]=ch;
    fclose(fp);
    DataBuff[cnt]='\0';
    printf("%s\n",DataBuff);
    system("pause");
}

void BuildTextFile(){
	FileData NewFile;
	cout<<"请输入文件名(无需后缀):"<<endl;
	cin>>NewFile.file_name;
	NewFile.file_name+=".txt";
	string file_address="./feihu";
	string TempString="";
	int TempCount = FileNameCount++;
	while(TempCount){
		TempString+=(char)(TempCount%10+'0');
		TempCount/=10;
	}
	reverse(TempString.begin(),TempString.end());
	file_address=file_address+TempString+".ion";
	NewFile.file_address=file_address;
	
	memset(DataBuff,0,sizeof(DataBuff));
	memset(AddressBuff,0,sizeof(AddressBuff));
	cout<<"请输入要存于该文件下的内容(以'$#'结尾加回车键结束输入):"<<endl;
	strcpy(AddressBuff,NewFile.file_address.c_str());
	FILE *fp = fopen(AddressBuff,"w");
	if(fp==NULL){
		cout<<"程序出现异常,将关闭程序"<<endl;
		system("exit");
	}
	
	//读入数据
	int cnt=0;
	char ch;
	bool flag=0;
	while(ch=getchar()){
		if(ch=='$'){
			flag=1;
			continue;
		}
		else if(flag&&ch=='#'){
			break;
		}else if(flag&&ch!='\n'){
			DataBuff[cnt++]='$';
			flag=0;
		}
		DataBuff[cnt++]=ch;
	}
	DataBuff[cnt]='\0';
	
	//去除文件首换行以及空格
	int StartIndex=0;
	while(DataBuff[StartIndex]==' '||DataBuff[StartIndex]=='\n')StartIndex++; 
	 
	fputs(DataBuff+StartIndex,fp);
	fclose(fp);
	FileRoom.push_back(NewFile);
	cout<<"文件创建成功,即将返回功能菜单......"<<endl;
	system("pause");
	//ShowFileContent(NewFile);
}

void OutputTextFileContentTo_FABU(FileData FD,HWND HD){
	if(HD==NULL)return ;
	memset(DataBuff,0,sizeof(DataBuff));
	memset(AddressBuff,0,sizeof(AddressBuff));
	
	strcpy(AddressBuff,FD.file_address.c_str());
	FILE* pFile = fopen(AddressBuff, "r");
	int cnt=0;
	char ch;
	while(~fscanf(pFile,"%c",&ch)){
		DataBuff[cnt++]=ch;
	}
	DataBuff[cnt]='\0';
	SendMessage(HD,WM_SETTEXT,0,(WPARAM)DataBuff);
	fclose(pFile);
	cout<<"\n完成!\n"<<endl;
	system("pause");
}

void ShowAllCurrentFiles(){
	cout<<"----------------------------------"<<endl;
	if(FileRoom.size()==0)cout<<"无可粘贴文件"<<endl;
	else cout<<"当前可粘贴文件如下所示:"<<endl;
	for(int i=0;i<FileRoom.size();i++)cout<<"文件"<<i+1<<":"<<FileRoom[i].file_name<<endl;
	cout<<"----------------------------------"<<endl<<endl;
}

int SelectTargetFileIndex(){
	
	if(FileRoom.size()==0){
		cout<<"请先构建文本文件!"<<endl;
		system("pause");
		return -1;
	}
	
	ShowAllCurrentFiles();
	
	int chioce;
	cout<<"请输入你想选择的文件序号:";
	cin>>chioce;
	return chioce-1;
}

HWND SearchSelectedHandle(){
	
	TargetHandleRoom.clear();
	EnumWindows(EnumWindowsProc, 0);
	
	if(TargetHandleRoom.size()==0){
		cout<<"未找到该窗口,将返回菜单......"<<endl;
		system("pause");
		return NULL;
	}
	
	int index=0;
	int maxH=0;
	for(int i=0;i<TargetHandleRoom.size();i++){
		RECT rect;  
		GetWindowRect(TargetHandleRoom[i],&rect); 	
		int w=rect.right-rect.left,h=rect.bottom-rect.top;
		if(w>0&&w<5000&&h>maxH){
			maxH=h;
			index=i;
		}
	}

	cout<<"该编辑窗口成功获取,即将输出文件内容至该窗口......"<<endl;
	return TargetHandleRoom[index];
}

void ShowMenuInterface(){
	while(1){
		system("cls");
		
		ShowAllCurrentFiles();
		cout<<"-----功能菜单-----"<<endl;
		cout<<"1.构建文本文件"<<endl;
		cout<<"2.(待拓展)"<<endl;
		cout<<"3.(待拓展)"<<endl;
		cout<<"4.将文本文件内容输出至<发布>中"<<endl;
		cout<<"5.退出程序"<<endl;
		cout<<"请输入功能序号:"<<endl;
		
		int chioce;
		cin>>chioce;
		switch(chioce){
			case 1:{
				BuildTextFile();
				break;
			}
			case 2:{
				
				break;
			} 
			case 3:{
				
				break;
			}
			case 4:{
				int index = SelectTargetFileIndex();
				if(index==-1)break;
				HWND HD = SearchSelectedHandle();
				system("pause");
				OutputTextFileContentTo_FABU(FileRoom[index],HD);
				break;
			}
			case 5:{
				return ;
				break;
			}
			default:{
				cout<<"异常输入!"<<endl;
				system("pause"); 
				break;
			}
		}
	}
}

void ShowTipsInterface(){
	cout<<"使用方法请参考 '使用说明' 文档"<<endl; 
	system("pause");
	system("cls");
}

void ProgramEncryption(){
	time_t tt = time(NULL);
	tm* t=localtime(&tt);
	unsigned long long hour = t->tm_hour;
	unsigned long long min = t->tm_min;
	unsigned long long ciphertext = ((hour<<3)^(hour<<5)&(hour<<1)+(min<<4))%33427057;
	ciphertext = ((ciphertext>>1)^(min<<7)^(min*32))%32743;
	//cout<<ciphertext<<endl;
	
	unsigned long long key;
	cout<<"请输入密钥:";
	cin>>key;
	
	if(key!=ciphertext){
		cout<<"密钥错误，将结束程序"<<endl;
		system("pause");
		exit(0);
		return ;
	}else{
		system("cls");
	}
}

int main(){
	//ProgramEncryption();
	ShowTipsInterface();
	ShowMenuInterface();
	return 0;
}
