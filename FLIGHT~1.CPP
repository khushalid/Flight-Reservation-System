#include<conio.h>
#include<string.h>
#include<fstream.h>
#include<stdio.h>

#define ROW 10
#define COL 4
class passenger;
/******************* CLASS FOR FLIGHT SCHEDULE *************/

class flight
{ long flightno;
  char source[15];
  char dest[15];
  int  tseats;
  int  booked;
  int  seatno[ROW][COL];
 public:
   flight();
   void readflight();
   void flightinfo() ;
   void showflight();
   void updateflight();
   void cancelation(int tpid,int tfno,int tsno);
   //accessor function
   int setbooked(int,int);

   int displayseatstatus(int sno=0,int pid=0);
   long getflightno();
   int cmpsrc(char src[20]);
   int cmpdest(char des[20]);
   //mutator function
   void setflightno(long ft);
};
void flight::cancelation(int tpid,int tfno,int tsno)
{
  fstream fio;
  int flag=0;
  fio.open("flight.dat",ios::in|ios::out|ios::binary);
  if(!fio)
   cout<<"\nFlight database missing!!!\nCancelation cancelled";
  else
  {
   while(fio.read((char*)this,sizeof(flight)))
   {
     if(flightno==tfno)
     {
       flag=1;
       break;
     }

   }
   if(flag==1 && seatno[tsno/COL][tsno%COL-1]==tpid)
   {
    seatno[tsno/COL][tsno%COL-1]=0;
    booked--;
    cout<<"\nNow Seat available for others";
    long pos=fio.tellg()-sizeof(flight);
    fio.seekp(pos,ios::beg);
    fio.write((char*)this,sizeof(flight));
   }
   else
     cout<<"\nFlight no does not exist or flight cancelled";
  fio.close();
 }
}
flight::flight()
{
  flightno=0;
  strcpy(source,"none");
  strcpy(dest,"none");
  tseats=40;
  booked=0  ;
  for(int i=0;i<ROW;i++)
  {
    for (int j=0;j<COL;j++)
    {
      seatno[i][j]=0;
    }
  }
}
void flight::readflight()
{
 cout<<"\nEnter the flight no.    : ";
 cin>>flightno;
 cout<<"\nEnter departure location: ";
 cin>>source;
 cout<<"\nEnter arrival location  : ";
 cin>>dest;
}
void flight::flightinfo()
{
  cout<<endl<<flightno<<"\t\t"<<source<<"\t\t"<<dest<<"\t\t"<<tseats<<"\t\t"<<tseats-booked<<endl;
}
void flight::showflight()
{
  cout<<"\nthe flight no. is        : "<<flightno;
  cout<<"\nthe departure location is: "<<source;
  cout<<"\nthe arrival location is  : "<<dest;
  cout<<"\ntotal seats              : "<<tseats;
  cout<<"\nbooked seats             : "<<booked ;
  cout<<"\navailable seats          : "<<tseats-booked;
  displayseatstatus();
}
void flight::updateflight()
{
 char s[20],d[20];
 cout<<"\nenter new departure location('.' if same): ";
 cin>>s;
 cout<<"\nenter new arrival location('.' if same)  : ";
 cin>>d;
 if(strcmp(s,".")!=0)
    strcpy(source,s);
 if(strcmp(d,".")!=0)
    strcpy(dest,d);
}
int flight::setbooked(int pid,int sno)
{
  int avail=tseats-booked;
  if(pid!=0)
  {
    if(avail>=1 && seatno[sno/COL][sno%COL-1]==0)
    {
      booked++;
      seatno[sno/COL][sno%COL-1]=pid;
    }
    else
      cout<<"\nseat already booked ";
  }
  else
  {
    if(seatno[sno/COL][sno%COL-1]!=0)
    {
      booked--;
      seatno[sno/COL][sno%COL-1]=pid;
    }
    else
      cout<<"\nNo booked seat found !!!";
    avail=tseats-booked;

  }
  return avail;
}
long flight::getflightno()
{
  return flightno;
}
int flight::cmpsrc(char src[20])
{
 return (strcmp(src,source));
}
int flight::cmpdest(char des[20])
{
  return (strcmp(des,dest));
}
void flight::setflightno(long ft)
{
  flightno=ft;
}

int flight::displayseatstatus(int sno,int pID)
{
  cout<<endl;
  for(int i=0;i<ROW;i++)
    for(int j=0;j<COL;j++)
    {
      if(sno==i*COL+j+1)
      {
	seatno[i][j]=pID;
      }
    }
   for(i=0;i<ROW;i++)
   {
     for(j=0;j<COL;j++)
     {
       if(seatno[i][j]==0)
	 if(i*COL+j+1<10)
	  cout<<i*COL+j+1<<"  ";
	 else
	  cout<<i*COL+j+1<<" ";
       else
	  cout<<"X  ";
      }
       cout<<endl;
   }
  return 0 ;
}




/*************** FUNCTION FOR ENTERING FLIGHT SCHEDULE********/

void ENTER_FLIGHT_DETAIL()
      {
      clrscr();
      cout<<"********************************************************************************";
      cout<<"\t\t\t\tENTER FLIGHT SCHEDULE\n";
      cout<<"********************************************************************************";
      char choice;
      do{
      flight f1;
      ofstream fout;
      fout.open("flight.dat",ios::out|ios::app|ios::binary);
      if(!fout)
	  cout<<"\nfile operation failed";
      else
	  {
	    f1.readflight();
	    fout.write((char*)&f1,sizeof(f1));
	    fout.close();
	  }
      cout<<"\nwant to enter more(y/n): ";
      cin>>choice;
      getch();
      }while(choice=='y'|choice=='Y');
}

/*********** FUNCTION TO SHOW FLIGHT INFO***************/

void displayflightschedule()
   {
   ifstream fin ;
   fin.open("flight.dat",ios::in|ios::binary);
   if(fin==NULL)
       cout<<"\nfile does not exist";
   else
      {
      flight f2;
      while(fin.eof()==0)
	  {
	  fin.read((char*)&f2,sizeof(f2));
	  if(fin.eof()!=0)
	       break;
	  f2.flightinfo();
	  getch();
	  }
      fin.close();
      }
   getch();
   }
/****************** to modify flight record*************/
void modifyflightrec()
   {
   ifstream fin ;
   fin.open("flight.dat",ios::in|ios::binary);
   ofstream fout;
   fout.open("temp.dat",ios::out|ios::binary) ;
   if(!fin||!fout)
      cout<<"\nmodification failed";
   else
      {
      flight f1;
      int no;
      cout<<"\nenter the flight no. to be modified: ";
      cin>>no;
      while(fin.read((char*)&f1,sizeof(f1)))
      {
	  if(f1.getflightno()==no)
	     {
	     f1.showflight();
	     f1.updateflight();
	     }
	  fout.write((char*)&f1,sizeof(f1));
      }
      fin.close();
      fout.close();
      remove("flight.dat");
      rename("temp.dat","flight.dat" );
      cout<<"\nmodification done";
      }
   }


/****************** CLASS FOR PASSENGER *************/

class passenger
{  int PID;
   char fname[15];
   char lname[15];
   int age;
   int sno;
   int fno;
   char gender[10];
   char email[30];
 public:
   void readdetail(int flightno)
   {
     fno=flightno;
     cout<<"\nenter passenger ID         : ";
     cin>>PID;
     cout<<"\nenter your first name      : ";
     cin>>fname;
     cout<<"\nenter your last name       : ";
     cin>>lname;
     cout<<"\nenter age                  : ";
     cin>>age;
     cout<<"\nenter your gender          : ";
     cin>>gender;
     cout<<"\nenter your email id        : ";
     cin>>email;
     cout<<"\nenter Seat No to be booked : ";
     cin>>sno;
     getch();
   }
   void showdetail()
  {
   cout<<"\nyour name is    : "<<fname<<" "<<lname;
   cout<<"\nyour age is     : "<<age;
   cout<<"\nyour gender is  : "<<gender;
   cout<<"\nyour email id is: "<<email;
   cout<<"\nyour seat no. is: "<<sno;
   cout<<"\nyour flight is  : "<<fno;
   getch();
   }
   void enternewpasrec()
   {
     char f[20],l[20],g[10],e[20];
     int a;
     cout<<"\n\nenter your first name('.' if same)      : ";
     cin>>f;
     cout<<"\nenter your last name('.' if same)       : ";
     cin>>l;
     cout<<"\nenter age('1' if same)                  : ";
     cin>>a;
     cout<<"\nenter your gender('.' if same)          : ";
     cin>>g;
     cout<<"\nenter your email id('.' if same)        : ";
     cin>>e;
     if(a!=1)
       age=a;
     if(strcmp(f,".")!=0)
       strcpy(fname,f);
     if(strcmp(l,".")!=0)
       strcpy(lname,l);
     if(strcmp(g,".")!=0)
       strcpy(gender,g);
     if(strcmp(e,".")!=0)
       strcpy(email,e);
     getch();
   }
     //accessor function
	int getpid();
	char *getlastname();
	int getage();
	char *getgender();
	char *getemail();
	int getflight();
	int getsno();
     //mutator function
	void setfname(char f[30])
	   {
	   strcpy(fname,f);
	   }
	void setlname(char l[30])
	   {
	   strcpy(lname,l);
	   }
	void setgender(char g[10])
	   {
	   strcpy(gender,g);
	   }
	void setemail(char e[50])
	   {
	   strcpy(email,e);
	   }
	void setage(int a)
	   {
	   age=a;
	   }
	void setflight(int fn)
	   {
	   fno=fn;
	   }
     };
int passenger::getpid()
   {
   return PID;
   }
char* passenger::getlastname()
   {
   return lname;
   }
int passenger::getage()
   {
   return age;
   }
char* passenger::getgender()
   {
   return gender;
   }
char* passenger::getemail()
   {
   return email;
   }
int passenger::getflight()
   {
   return fno;
   }
int passenger::getsno()
   {
   return sno;
   }
/*************** FUNCTION FOR ENTERING PASSENGER DETAIL************/

void ENTER_PASSENGER_DETAIL()
    {
    int fno;
    ofstream fout;
    fout.open("passenger.dat",ios::out|ios::app|ios::binary);
    if(!fout)
	 cout<<"\nfile does not exist";
    else
	{
	     fstream finout;
	     finout.open("flight.dat",ios::in|ios::out|ios::binary);
	     if(!finout)
		cout<<"\nfile does not exist";
	     else
		{
		 flight f1;
		 int flag=0;
		 cout<<"\nenter the flight no. to be searched: ";
		 cin>>fno;
		 while(finout.read((char*)&f1,sizeof(f1)))
		   {
		   if(f1.getflightno()==fno)
		      {
		      flag=1;
		      break;
		      }
		   }
		if(flag==1)
		  {  cout<<"\nTHE FLIGHT DETAILS ARE:\n";
		     f1.showflight();
		     passenger p1;
		     p1.readdetail(fno);
		     fout.write((char*)&p1,sizeof(p1));
		     fout.close();

		     f1.setbooked(p1.getpid(),p1.getsno());
		     long pos=finout.tellg()-sizeof(f1);
		     finout.seekg(pos);
		     finout.write((char*)&f1,sizeof(f1));
		     cout<<"\nyour record saved successfully";
		   }
		else
		   {

		   cout<<"\nNO FLIGHT FOUND!!!";
		   }
		finout.close();
		}
	      }
    }

/****************** FUNCTION TO SHOW PASSENGER RECORD *****************/

void SHOW_PASSENGER_DETAIL()
    {
    ifstream fin;
    fin.open("passenger.dat",ios::in|ios::binary);
    if(!fin)
       cout<<"\nfile does not exist";
    else
      {
      passenger p1;
      while(!fin.eof())
	 {
	 fin.read((char*)&p1,sizeof(p1));
	 if(fin.eof()!=0)
	   break;
	 p1.showdetail();
	 }
      fin.close();
      }
    }
/**************** function to modify passenger record*********/
void modifypasrec()
{
   ifstream fin("passenger.dat",ios::in|ios::binary);
   ofstream fout("temp.dat",ios::out|ios::binary);
   if(!fin||!fout)
      cout<<"\nmodification failed";

   else
      {
      passenger p1;
      int id;
      cout<<"\nenter the passenger ID to be modified: ";
      cin>>id;
      while(fin.read((char*)&p1,sizeof(p1)))
	  {
	  if(p1.getpid()==id)
	     {
	     cout<<"\nrecord found";
	     p1.showdetail();
	     p1.enternewpasrec();
	     fout.write((char*)&p1,sizeof(p1));
	     cout<<"\nMODIFICATION DONE :)";
	     getch();
	     }
	  else
	     fout.write((char*)&p1,sizeof(p1));
	  }
      fin.close();
      fout.close();
      remove("passenger.dat");
      rename("temp.dat","passenger.dat" );
    }
   getch();
   }
/***************FUNCTION FOR CANCELLING RESERVATION***************/
void cancel_res()                                   //passenger id, flight no
{
   ifstream fin("passenger.dat",ios::in|ios::binary);
   ofstream fout("temp.dat",ios::out|ios::binary);
   if(!fin||!fout)
      cout<<"\ncancellation  failed";

   else
      {
      flight f1;
      passenger p1;
      int id;
      int found=0;// only one cancelation take place at a time.
      cout<<"\nenter the passenge ID to be cancelled: ";
      cin>>id;
      while(fin.read((char*)&p1,sizeof(p1)))
	  {
	  if(p1.getpid()==id && found==0)
	     {
	       cout<<"\nrecord deleted";
	       p1.showdetail();
	       f1.cancelation(p1.getpid(),p1.getflight(),p1.getsno());
	       found=1;
	       cout<<"\nCANCELLLATION DONE :)";
	     }
	  else
	    {cout<<"\nNO RECORD FOUND";
	     fout.write((char*)&p1,sizeof(p1));
	     }
	  }
      fin.close();
      fout.close();
      remove("passenger.dat");
      rename("temp.dat","passenger.dat" );

      }
   getch();
   }
void bookingstatus()
   {
   passenger p1;
   int no,Pid;
   char src[20],des[20];
   long flno;
   ifstream fin("flight.dat",ios::in|ios::binary) ;
   if(!fin)
      {
      cout<<"\n file does not exist";
      }
   else
      {
      int found=0;
      flight f1;
      cout<<"\nenter the source location      :";
      cin>>src;
      cout<<"\nenter the destination location :";
      cin>>des;
      cout<<"\nenter the flight number        :";
      cin>>flno;
      while(fin.read((char*)&f1,sizeof(f1)))
	 {
	 if(f1.cmpsrc(src)==0&&f1.cmpdest(des)==0&&f1.getflightno()==flno)
	    {
	    found=1;
	    break;
	    }
	 }
      if(found==1)
	 {
	 f1.showflight();
	 }
      else
	 cout<<"\nrecord not found";
      }
   getch();
   }
class administrator
  {
    char uname[30];
    char pass[15];
  public:
    void tdetail()
      {
	cout<<"\nyour username: ";
	cin>>uname;
	cout<<"\ncreate password: ";
	cin>>pass;
      }
    void sdetail()
      {
       cout<<"\nusername: "<<uname;
       cout<<"\npassword: "<<pass;
      }
    char* getuname()
      {
      return uname;
      }
    char* getpass()
      {
      return pass;
      }
    };
void id()
   {
   clrscr();
   ofstream fout;
   fout.open("id.dat",ios::app);
   if(!fout)
     cout<<"\nfile does not exist";
   else
     {
      administrator a1;
      a1.tdetail();
      fout.write((char*)&a1,sizeof(a1));
     }
   fout.close();
   getch();
   }
int sid()
   {
   clrscr();
   ifstream fin;
   fin.open("id.dat");
   if(!fin)
     cout<<"\nno file";
   else
     {
     char n[30],p[30];
     administrator a1;
     cout<<"\nenter username: ";
     cin>>n;
     cout<<"\nenter password: ";
     cin>>p;
     while(fin.read((char*)&a1,sizeof(a1)))
	  {
	  if(a1.getuname()==n&&a1.getpass()==p)
	     {
	     return 1;
	     }
	  }
     fin.close();
     getch();
     }
     return 0;
   }


void main()
    {
    int choice;
    passenger p;
    int ch,c;
    do{
     clrscr();
     cout<<"********************************************************************************";
     cout<<"\t\tTHIS IS A AIRPLANE SEAT RESERVATION PROGRAM\n";
     cout<<"********************************************************************************";
     cout<<"\nMAIN MENU";
     cout<<"\n==========";
     cout<<"\n1.CREATE ACCOUNT";
     cout<<"\n2.ADMINISTER LOG IN"<<"\n3.BOOK A FLIGHT AND PASSENGER DETAIL"<<"\n4.EXIT";
     cout<<"\nYOUR CHOICE(1-4): ";
     cin>>ch;
     switch(ch)
     {
	case 1:
	       id();
	       break;
	case 2: int r;
		r=sid();
		if(r==1)
		{
		do{
		clrscr();
		cout<<"\n1. ENTER FLIGHT SCHEDULE";
		cout<<"\n2. MODIFY FLIGHT SCHEDULE";
		cout<<"\n3. EXIT";
		cout<<"\nYOUR CHOICE(1-3): ";
		cin>>c;
		switch(c)
		   {
		   case 1:
			  clrscr();
			  cout<<"********************************************************************************";
			  cout<<"\t\t\t\tENTER FLIGHT SCHEDULE\n";
			  cout<<"********************************************************************************";
			  ENTER_FLIGHT_DETAIL();
			  break;
		   case 2:clrscr();
			  cout<<"********************************************************************************";
			  cout<<"\t\t\tMODIFY FLIGHT RECORD\n";
			  cout<<"********************************************************************************";
			  modifyflightrec();
			  break;

		   }
		}while(c!=3);
		}
		else
		   cout<<"\nNO SUCH RECORD";
		break;
	case 3:
		 do{
		 clrscr();
		 cout<<"\n1. FLIGHT INFORMATION     ";
		 cout<<"\n2. SEAT BOOKING";
		 cout<<"\n3. MODIFY PASSENGER RECORD";
		 cout<<"\n4. CANCELLING RESERVATION";
		 cout<<"\n5. FLIGHT OCCUPANCY & PASSENGER DETAIL  ";
		 cout<<"\n6. SEAT STATUS";
		 cout<<"\n7. EXIT";
		 cout<<"\nENTER YOUR CHOICE NO.(1-7): ";
		 cin>>choice;
		 switch(choice)
		    {
		      case 1:
			     clrscr();
			     cout<<"********************************************************************************";
			     cout<<"\t\t\t\tFLIGHT INFORMATION\n";
			     cout<<"********************************************************************************";
			     cout<<"FLIGHT NO.\tDEPARTURE LOC.\tARRIVAL LOC.\tTSEATS\t\tAVAILABLE";
			     cout<<"\n----------------------------------------------------------------------";
			     displayflightschedule();
			     break;
		      case 2:
			     clrscr();
			     cout<<"********************************************************************************";
			     cout<<"\t\t\t\tSEAT BOOKING\n";
			     cout<<"********************************************************************************";
			     ENTER_PASSENGER_DETAIL();
			     break;
		      case 3 :
			     clrscr();
			     cout<<"********************************************************************************";
			     cout<<"\t\t\t\tMODIFY PASSENGER RECORD\n";
			     cout<<"********************************************************************************";
			     modifypasrec();
			     break;
		      case 4:
			     clrscr();
			     cout<<"********************************************************************************";
			     cout<<"\n\t\t\t\tCANCELING RESERVATION\n";
			     cout<<"\*******************************************************************************";
			     cancel_res();
			     break;
		      case 5:clrscr();
			     cout<<"********************************************************************************";
			     cout<<"\n\t\t\t\tPASSENGER DETAIL\n";
			     cout<<"********************************************************************************";
			     SHOW_PASSENGER_DETAIL();
			     break;
		      case 6:clrscr();
			     cout<<"********************************************************************************";
			     cout<<"\t\t\t\tSEAT STATUS\n";
			     cout<<"********************************************************************************";
			     bookingstatus();
		    }
		 }while(choice!=7);
	}
     }while(ch!=4);
   getch();
}