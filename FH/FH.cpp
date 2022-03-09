#include <iostream>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <time.h>
using namespace std;

int FileNameCount=1;//�ļ��洢�±������ 
char DataBuff[25500];//�ļ����ݻ����� 
char NameBuff[255];//�ļ����������� 
char AddressBuff[255];//�ļ���ַ������ 
void ShowSize(HWND);//չʾ����ߴ��С 

typedef struct Name{
	string file_address;//�ļ����ڵĵ�ַ 
	string file_name;//�û��������ļ��� 
}FileData;
vector<FileData> FileRoom;//�ļ��洢�б� 
vector<HWND> TargetHandleRoom;//Ŀ����ջ 

BOOL CALLBACK EnumWindowsProc(HWND, LPARAM);//�ҵ����������ھ�� 
BOOL CALLBACK EnumChildProc(_In_ HWND, _In_ LPARAM);//�ҵ����������Ӵ��ڵĿؼ�������� 
void ShowSize(HWND);//չʾ������ڵĳߴ��С
void ShowFileContent(FileData);//��ʾ�ļ�����
void BuildTextFile();//�������ļ�����������
void OutputTextFileContentTo_FABU(FileData,HWND);//���ļ���������������� 
void ShowAllCurrentFiles();//��ʾ��ǰ�ѹ����������ļ�
int SelectTargetFile();//��ȡָ�����ļ����ļ��б��е��±� 
HWND SearchSelectedHandle();//��ȡĿ���� 
void ShowMenuInterface();//��ʾ�˵�����
void ShowTipsInterface();//��ʾ��ʾ���� 
void ProgramEncryption();//����α����(����ʹ��) 

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
	cout<<"handle: "<<hq<<"    ��:"<<w<<" "<<"��:"<<h<<endl;
}

void ShowFileContent(FileData FD){
	//������� 
	memset(DataBuff,0,sizeof(DataBuff));
	memset(AddressBuff,0,sizeof(AddressBuff));
	strcpy(AddressBuff,FD.file_address.c_str());
    int cnt=0;
    char ch;
    FILE *fp=fopen(AddressBuff,"r");
    if(fp==NULL){
		cout<<"��������쳣,���رճ���"<<endl;
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
	cout<<"�������ļ���(�����׺):"<<endl;
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
	cout<<"������Ҫ���ڸ��ļ��µ�����(��'$#'��β�ӻس�����������):"<<endl;
	strcpy(AddressBuff,NewFile.file_address.c_str());
	FILE *fp = fopen(AddressBuff,"w");
	if(fp==NULL){
		cout<<"��������쳣,���رճ���"<<endl;
		system("exit");
	}
	
	//��������
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
	
	//ȥ���ļ��׻����Լ��ո�
	int StartIndex=0;
	while(DataBuff[StartIndex]==' '||DataBuff[StartIndex]=='\n')StartIndex++; 
	 
	fputs(DataBuff+StartIndex,fp);
	fclose(fp);
	FileRoom.push_back(NewFile);
	cout<<"�ļ������ɹ�,�������ع��ܲ˵�......"<<endl;
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
	cout<<"\n���!\n"<<endl;
	system("pause");
}

void ShowAllCurrentFiles(){
	cout<<"----------------------------------"<<endl;
	if(FileRoom.size()==0)cout<<"�޿�ճ���ļ�"<<endl;
	else cout<<"��ǰ��ճ���ļ�������ʾ:"<<endl;
	for(int i=0;i<FileRoom.size();i++)cout<<"�ļ�"<<i+1<<":"<<FileRoom[i].file_name<<endl;
	cout<<"----------------------------------"<<endl<<endl;
}

int SelectTargetFileIndex(){
	
	if(FileRoom.size()==0){
		cout<<"���ȹ����ı��ļ�!"<<endl;
		system("pause");
		return -1;
	}
	
	ShowAllCurrentFiles();
	
	int chioce;
	cout<<"����������ѡ����ļ����:";
	cin>>chioce;
	return chioce-1;
}

HWND SearchSelectedHandle(){
	
	TargetHandleRoom.clear();
	EnumWindows(EnumWindowsProc, 0);
	
	if(TargetHandleRoom.size()==0){
		cout<<"δ�ҵ��ô���,�����ز˵�......"<<endl;
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

	cout<<"�ñ༭���ڳɹ���ȡ,��������ļ��������ô���......"<<endl;
	return TargetHandleRoom[index];
}

void ShowMenuInterface(){
	while(1){
		system("cls");
		
		ShowAllCurrentFiles();
		cout<<"-----���ܲ˵�-----"<<endl;
		cout<<"1.�����ı��ļ�"<<endl;
		cout<<"2.(����չ)"<<endl;
		cout<<"3.(����չ)"<<endl;
		cout<<"4.���ı��ļ����������<����>��"<<endl;
		cout<<"5.�˳�����"<<endl;
		cout<<"�����빦�����:"<<endl;
		
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
				cout<<"�쳣����!"<<endl;
				system("pause"); 
				break;
			}
		}
	}
}

void ShowTipsInterface(){
	cout<<"ʹ�÷�����ο� 'ʹ��˵��' �ĵ�"<<endl; 
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
	cout<<"��������Կ:";
	cin>>key;
	
	if(key!=ciphertext){
		cout<<"��Կ���󣬽���������"<<endl;
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
