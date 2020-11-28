#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<string.h>
#include<stack>
#include<algorithm>
#include<windows.h>
#include<stdlib.h>
#include<math.h>
#include<conio.h>
#define P 0.02
#define TOL 3
#define PATH "D:/programs_cpp/package_ads_sem3/movie3.csv"

using namespace std;

const int letters= 26;

const long long int mod=pow(2,32);

long long int fnv_hash_value=2166136261LL;
long long int murmur_hash_value;

void mhash3(long long int,int);
void fnv_hash_1(long long int);

stack<int> gen,rat;
void assign_hash(long long int h)
{
    murmur_hash_value=h;
}
string conv_to_hexa(long long int x)
{
    int temp=0;
    int i=0;
    int flag=1;
    string s="";
    while(flag)
    {
            if( x < 16)
                    flag=0;
            temp=x%16;
            if( temp > 9)
                s+= char(10 + temp);
            else
                    s+=48+temp;
            x/=16;
    }
    return s;
}
long long int conv_to_decimal(string s)
{
    long long int temp=0;
    int i=0;
    int f;
    while( i < s.length())
    {
        f=s[i];
        if( f < 97)
            temp += ( (f - 48)* pow(16,i));
        else
            temp += (f-87) * pow(16,i);
        i++;
    }
    return temp;
}
void conv_to_k(string s,int flag)
{
    int i=0;
    string temp="";
    long long int k;
    while(i < s.length())
            temp+=conv_to_hexa(s[i++]);
    k=conv_to_decimal(temp);
    if(flag == 1)
        mhash3(k,s.length());
    else
        fnv_hash_1(k);
}
void mhash3(long long int k,int len)
{
    k=(k * 3432918353LL) % mod;
    k= (( k << 15) | (k >> 32 - 15)) ;
    k %= mod;
    k= (k * 461845907LL) % mod;
    murmur_hash_value=(murmur_hash_value ^k) % mod;

    if(len == 4)
    {
        murmur_hash_value = ((murmur_hash_value << 13) | (k >> 32 - 13)) ;
        murmur_hash_value %= mod;
        murmur_hash_value = (murmur_hash_value * 5 + 3864290660LL) % mod;
    }
    else
    {
        murmur_hash_value= (murmur_hash_value ^ len) ;
        murmur_hash_value %= mod;
    }
}
void fnv_hash_1(long long int k)
{
    fnv_hash_value *= 16777619LL;
    fnv_hash_value %= mod;
    fnv_hash_value=fnv_hash_value ^ k;
    fnv_hash_value %= mod;
}
void final_avalanche()
{
    murmur_hash_value = murmur_hash_value ^(murmur_hash_value >> 16);
    murmur_hash_value %= mod;
    murmur_hash_value *= 2246822507LL ;
    murmur_hash_value %= mod;
    murmur_hash_value = murmur_hash_value ^(murmur_hash_value >>13);
    murmur_hash_value %= mod;
    murmur_hash_value *=  3266489909LL;
    murmur_hash_value %= mod;
    murmur_hash_value = murmur_hash_value ^(murmur_hash_value >> 16);
    murmur_hash_value %= mod;
}
void string_manip(string s,int flag)
{
    int i=0;
    string temp="";
    while( i < s.length())
    {
        if((i+4) > s.length())
            temp=s.substr(i,4);
        else
            temp=s.substr(i);
        i+=4;
        conv_to_k(temp,flag);
    }
    final_avalanche();
}
class blm_fltr
{
    int f_size;
    float err_tol;
    int n_hash;
    int n_entries;
    vector<int> filter;

    public: blm_fltr(int n)
            {
                n_entries=n;
                err_tol=P;
                f_size = - (n* log(P)) / (pow(log(2),2));
                n_hash = (f_size/n) * log(2);
                for(int i=0;i<f_size;i++)
                    filter.push_back(0);
            }
            blm_fltr( const blm_fltr &temp)
            {
                f_size = temp.f_size;
                err_tol = temp.err_tol;
                n_hash = temp.n_hash;
                n_entries = temp.n_entries;
                filter = temp.filter;
            }

            void insert_value(string s)
            {
                int temp=n_hash;
                while(temp > 0)
                {
                    assign_hash(temp+100);
                    string_manip(s,1);
                    filter[murmur_hash_value % f_size] = 1;
                    temp--;
                }
            }
            int check_word(string );
};
int blm_fltr::check_word(string s)
{
    int temp=n_hash ;
    while(temp > 0)
    {
        assign_hash(temp+100);
        string_manip(s,1);
        if(filter[murmur_hash_value % f_size] == 0)
            return 1;
        temp--;
    }
    if( temp == 0)
        return 0;
}
blm_fltr create_filter()
{
    int n=0;
    string line;
    ifstream fin(PATH);
    while(getline(fin,line))
        n++;
    fin.close();
    blm_fltr temp(n);
    return temp;
}
int min_num(int i, int j)
{
    if( i < j)
        return i;
    else return j;
}
int max_num(int i, int j)
{
    if( i > j)
        return i;
    else return j;
}
class llist;
class info_node
{
    //friend class bk_node;
    friend class BK_tree;
    friend  void Get_rcmnds(llist** ,blm_fltr&, string* ,int ,int);
    int rating;
    int genre;
};
class bk_node
{
    string s;
    int levd;
    info_node* last;
    vector<bk_node* > arr;
    friend class BK_tree;
};
class BK_tree
{
    bk_node* root;
    int tolerance;
    info_node* s_word;
    int g_flag;
    int index;

    public: BK_tree()
            {
                g_flag=0;
                index=1;
                tolerance=TOL;
                root = NULL;
            }
            void set_word(info_node* p)
            {
                s_word=p;
            }
            info_node* search_word(string *s)
            {
                tolerance=0;
                get_words(s);
                tolerance=TOL;
                return s_word;
            }
            void insert_node(string s,int r,int g)
            {
                insert_node(root,s,r,g);
            }
            void insert_node(bk_node* ,string ,int ,int );
            int get_words(string *str)
            {
                index=1;
                g_flag=0;
                return get_words(root,str);
            }
            int get_words(bk_node* , string* );
            int lev_dis(string ,string);
};
int BK_tree::lev_dis(const string a, const string b)
{
    const int x=a.length()+1;
    const int y=b.length()+1;
    int mat[x][y];
    for(int i=0;i<x;i++)
        for(int j=0;j<y;j++)
            mat[i][j]=0;

    for(int i=1;i<x;i++)
        mat[i][0] = i;
    for(int j=1;j<y;j++)
        mat[0][j] = j;

    int cost=0;
    for(int i=1;i<x;i++)
    {
        for(int j=1;j<y;j++)
        {
            if(a[i-1] == b[j-1])
                cost=0;
            else
                cost=1;

            int k=min_num(mat[i-1][j] +1 ,mat[i][j-1] +1);
            mat[i][j]=min_num(k ,mat[i-1][j-1] + cost);
        }
    }
    return mat[x-1][y-1];
}
int BK_tree::get_words(bk_node* p, string *str)
{
    if( p == NULL)
        return 0;
    string s_temp=p->s;
    int l = lev_dis(s_temp,str[0]);
    if(s_temp == str[0]){
        set_word(p->last);
    }
    if( l <= tolerance){
        if(tolerance == 0)
            g_flag=1;
        str[index++]=p->s;
        return 0;
    }
    int high = l + tolerance;
    int low = l - tolerance;
    if( low <= 0)
        low=1;
    bk_node* temp=new bk_node;
    for(int i=high;i>=low;i--)
    {
        if(g_flag == 1)
            return 0;
        temp = NULL;
        for(int j=0;j<p->arr.size();j++)
        {
            if( i == p->arr[j]->levd )
            {
                temp=p->arr[j];
                break;
            }
        }
        if( temp != NULL )
            get_words(temp,str);
    }
    return index;
}
void BK_tree::insert_node(bk_node* p,string str,int r,int g)
{
    bk_node* t=new bk_node;
    t->s=str;
    t->last=new info_node;
    t->last->genre=g;
    t->last->rating=r;
    if( p == NULL)
    {
        t->levd=0;
        root = t;
        return;
    }
    string s_temp=p->s;
    int l=lev_dis(s_temp,str);
    t->levd=l;
    int i;
    for(i=0;i<p->arr.size();i++)
    {
        if(p->arr[i]->levd == l)
        {
            insert_node(p->arr[i] ,str,r,g);
            return;
        }
    }
    p->arr.push_back(t);
}

BK_tree bk;

class node
{
    string name;
    int no_of_g;
    int year;
    node* next;
    friend class llist;
};

class llist
{
    node* root;
    public:

    llist()
    {
        root=NULL;
    }
    void insert_node(string,int,int);
    int display_y_g(string s_name,int flag)
    {
        node* p= root;
        while( p != NULL)
        {
            if(p->name == s_name && flag == 1){
                cout<<p->year<<endl;
                break;
            }
            else if( p->name == s_name && flag == 2)
                return 1;
            p=p->next;
        }
        return 0;
    }
    string next(blm_fltr &f,int year1,int year2)
    {
        node* p=root;
        while(p!=NULL)
        {
            if(year1 == 1 && f.check_word(p->name)){
                f.insert_value(p->name);
                return p->name;
            }
            else if(f.check_word(p->name) && p->year>=year1 && p->year <= year2){
                f.insert_value(p->name);
                return p->name;
            }
            p=p->next;
        }
        if(p == NULL)
            return "";
    }
};
void llist::insert_node(string name,int year,int no_of_g)
{
    node* p;
    p=new node;

    p->year = year;
    p->name = name;
    p->no_of_g = no_of_g;
    p->next = NULL;

    if(root == NULL)
    {
        root=p;
        return;
    }
    node* temp=root;
    node* trav=NULL;
    while(1)
    {
        if(temp!= NULL && year >= temp->year )
            break;
        else if(temp == NULL)
            break;
        trav=temp;
        temp=temp->next;
    }
    if(trav== NULL)
    {
        p->next=temp;
        root=p;
    }
    else
    {
        trav->next=p;
        p->next=temp;
    }
}
int max_year=0,min_year=3000;
void genre_select(int x)
{
    switch(x)
    {
        case 1: cout<<"Action, ";
                break;
        case 2: cout<<"Adult,";
                break;
        case 3: cout<<"Adventure, ";
                break;
        case 4: cout<<"Animation, ";
                break;
        case 5: cout<<"Biography, ";
                break;
        case 6: cout<<"Comedy, ";
                break;
        case 7: cout<<"Crime, ";
                break;
        case 8: cout<<"Documentary, ";
                break;
        case 9: cout<<"Drama, ";
                break;
        case 10: cout<<"Family, ";
                 break;
        case 11: cout<<"Fantasy, ";
                 break;
        case 12: cout<<"Film Noir, ";
                 break;
        case 13: cout<<"Game Show, ";
                 break;
        case 14: cout<<"History, ";
                 break;
        case 15: cout<<"Horror, ";
                 break;
        case 16: cout<<"Musical, ";
                 break;
        case 17: cout<<"Mystery, ";
                 break;
        case 18: cout<<"News, ";
                 break;
        case 19: cout<<"Reality TV, ";
                 break;
        case 20: cout<<"Romance, ";
                 break;
        case 21: cout<<"SciFi, ";
                 break;
        case 22: cout<<"Short, ";
                 break;
        case 23: cout<<"Sport, ";
                 break;
        case 24: cout<<"TalkShow, ";
                 break;
        case 25: cout<<"Thriller, ";
                 break;
        case 26: cout<<"War, ";
                 break;
        case 27: cout<<"Western, ";
                 break;
    }
}
blm_fltr initialize_data(llist **h)
{
    ifstream fin(PATH);
    string line,temp,name;
    blm_fltr b_f = create_filter();
    int len,rating,i,year;
    int f,next;
    getline(fin,line);
    while(getline(fin,line))
    {
        len=line.length();
        f=line.find("(");
        temp=line.substr(0,f-1);
        if(temp[0] == ' ')
            temp=temp.substr(1);
        name=temp;
        std::transform(name.begin(),name.end(),name.begin(),::tolower);
        b_f.insert_value(name);
        f=line.find(",");
        next=line.find(",",f+1);
        temp=line.substr(f+1,next - f-1);
        rating= (temp[0] - 48)*10;
        if(temp.length() > 1)
            rating+= (temp[2] - 48);
        temp="";

        f=line.find(",",next+1);
        temp=line.substr(next+1 , f-next-1);
        year = 0;
        i=0;
        while(i!=4){
            year += ((temp[i] - 48) * int(pow(10,3-i)));
            i++;
        }
        temp="";
        if(max_year < year)
            max_year=year;
        if(min_year > year)
            min_year=year;
        next=line.find(",",f+1);
        temp=line.substr(f+1,next-f-1);
        int no_of_g=temp[0]-48;

        temp="";
        i=0;
        f=line.find(",",next+1);
        int flag=0;
        while( 1)
        {
            temp=line.substr(next+1,1);
            if(temp[0] - 48 == 1){
                h[rating][i].insert_node(name,year,no_of_g);
                if(flag == 0)
                {
                    bk.insert_node(name,rating,i);
                    flag=1;
                }
            }
            next=f;
            f=line.find(",",f+1);
            if( f == string::npos)
            {
                temp=line.substr(next+1,1);
                if(temp[0] - 48 == 1){
                    h[rating][i].insert_node(name,year,no_of_g);
                    if(flag == 0)
                    {
                        bk.insert_node(name,rating,i);
                        flag=1;
                    }
                }

                break;
            }
            i++;
        }
        name="";
        temp="";
    }
    return b_f;
}
string ChK_mv_val(blm_fltr f)
{
    system("cls");
    string line[10];
    int i=0,j,choice=0;
    cout<<"\nEnter movie:";
    cin.ignore();
    getline(cin,line[0]);
    std::transform(line[0].begin(),line[0].end(),line[0].begin(),::tolower);
    if ( f.check_word(line[0]) )
    {
        i=bk.get_words(line);
        cout<<"Sorry, no such movie in our Database"<<endl;
        if(i>=2){
        cout<<"Did you mean :-"<<endl;
            j=1;
            while(j < i )
                cout<<j-1<<". "<<line[j++]<<endl;
        }
        cout<<"If you want to search online for "<<line[0]<<" press 0 or else press any of the suggestions(if none available press 0)"<<endl;
        cout<<"Enter your choice:";
        cin>>choice;
        if(choice==0)
            return line[0];
    }
    else
        i+=2;
    if(i<2)
        return "";
    else
        return line[choice];
}
void Search_online(string temp2)
{
    int i=0;
    string temp1 = "https://en.wikipedia.org/wiki/";
    while(i < temp2.length()){
        if(temp2[i] == ' ')
            temp2[i]='_';
        i++;
    }
    temp1=temp1+temp2;
    HWND hwnd = GetDesktopWindow();
    ShellExecute(hwnd, "open",temp1.c_str() , NULL, NULL, NULL);
}
void Get_rcmnds(llist **h,blm_fltr &mv_load,string *temp,int year1,int year2)
{
    system("cls");
    int rr=0;
    int r_size,r,g=2,i=0;
    int r_arr[rat.size()+1];
    int g_arr[28]={0};
    if(temp[0] != ""){
        info_node* p=bk.search_word(temp);
        cout<<"\nMovie name:"<<temp[0];
        cout<<"\nRating:"<<p->rating/10<<"."<<p->rating%10;
        cout<<"\nYear of Release:";
        h[p->rating][p->genre].display_y_g(temp[0],1);
        cout<<"Genre:";
        while(i<28)
        {
            if(h[p->rating][i++].display_y_g(temp[0],2) )
                genre_select(i);
        }
        cout<<"\n\nRecommended movies :-\n\n";
        r=p->rating;
        gen.push(p->genre);
        rat.push(p->rating);
        for(i=0;i<rat.size();i++){
            r_arr[i]=rat.top();
            rat.pop();
        }
        r_size=i;
    }
    else
    {
        for(i=0;i<rat.size()+1;i++)
            r_arr[i]=(80+i)%100;
        r_size=i;
        r=r_arr[0];
        rr++;
    }
    for(i=0;i<gen.size();i++)
        g_arr[gen.top()]++;
    i=0;
    int j=0;
    while(i<5)
    {
        for(;j<28;j++)
        {
            if(g_arr[j]!=0)
            {
                g_arr[j]--;
                g=j;
                break;
            }
        }
        if(j==28)
            j=0;
        string s=h[r][g].next(mv_load,year1,year2);
        if(s != "")
            cout<<++i<<". "<<s<<endl;
        if(rr < r_size){
            r=r_arr[rr++];
        }
        else
            r=(r+1)%100;
    }
    rr--;
    if(temp[0] != ""){
        while(rr>-1)
            rat.push(r_arr[rr--]);
    }
    cout<<"\nPRESS ANY TO CONTINUE";
    _getch();
}
void Show_Credits()
{
    system("cls");
    cout<<"\t\t\t\t\t\t\t\t\t\t*****CREDITS*****";
    cout<<"\n\t\t\t\t\t\t\t\t\t\t18PD30 - S. Rooban\n\t\t\t\t\t\t\t\t\t\t18PD02 - M. Abhirami";
    cout<<"\n\n\t\t\t\t\t\t\t\t\t  *****DATA STRUCTURES USED*****";
    cout<<"\n\t\t\t\t\t\t\t\t\t\t   Linked list\n\t\t\t\t\t\t\t\t\t\t    BK tree\n\t\t\t\t\t\t\t\t\t\t   bloom filters";
    cout<<"\n\n\t\t\t\t\t\t\t\t\t     *****ALGORITHMS USED*****";
    cout<<"\n\t\t\t\t\t\t\t\t\t\t    MurmurHash\n\t\t\t\t\t\t\t\t\t\t     FNV hash\n\t\t\t\t\t\t\t\t\t     Levenshtein edit distance";
    cout<<"\n\n\t\t\t\t\t\t\t\t\t\t*****NOTE*****\n\n\n\t\t\t\t\t\t\t\t\tDataset - imdb movies, taken from kaggle and modified\n\n\t\t\t\t\t\t\t\t\tRecommendations based on rating and genre by default\n\n\t\t\t\t\t\t\t\t\tNo two same movies will be repeated again for recommendations\n\n\t\t\t\t\t\t\t\t\tFitted with spell checking with the custom dictionary\n\n\t\t\t\t\t\t\t\t\tCustom seeds used for hashing";
    cout<<"\n\n\t\t\t\t\t\t\t\t\tPRESS ANY KEY TO RETURN";
    _getch();
}
int main()
{
    int choice=0,i;
    llist **h;
    h = new llist *[100];
    for(i = 0; i < 100; i++)
        h[i] = new llist[28];
    i=0;
    string temp[2];
    blm_fltr mvc = initialize_data(h);
    blm_fltr mv_load = create_filter();
    cout<<"\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\tWelcome to your own personal Movie Recommender!!\n\t\t\t\t\t\t\t\t\t\t\t\tAre you ready?\n\t\t\t\t\t\t\t\t\t\t\tPRESS ANY KEY TO CONITNUE";
    _getch();
    while(choice != 5)
    {
        system("cls");
        cout<<"\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t1.Enter movie\n\t\t\t\t\t\t\t\t\t\t2.Get recommendations\n\t\t\t\t\t\t\t\t\t\t3.Re-initialize all data\n\t\t\t\t\t\t\t\t\t\t4.Credits and important notes\n\t\t\t\t\t\t\t\t\t\t5.Exit";
        cout<<"\n\t\t\t\t\t\t\t\t\t\tEnter your choice:";
        cin>>choice;
        if(choice>=1 && choice<=5)
        {
            switch(choice)
            {
                case 1:
                {
                    temp[0]=ChK_mv_val(mvc);
                    if(temp[0] != "" && !mvc.check_word(temp[0])){
                        mv_load.insert_value(temp[0]);
                        Get_rcmnds(h,mv_load,temp,1,1);
                    }
                    else if(temp[0] != "")
                        Search_online(temp[0]);
                    break;
                }
                case 2:
                {
                    string temp[1];
                    temp[0]="";
                    choice=2;
                    while(choice!=0){
                        system("cls");
                        cout<<"\nWould u prefer to choose the range of years for the movies?(Enter 1 for yes and 0 for no)";
                        cin>>choice;
                        if(choice){
                            int year1,year2;
                            cout<<"\nWe have movies ranging from "<<min_year<<" to "<<max_year<<", please choose the range appropriately"<<endl;
                            cout<<"Example: 2000 - 2010"<<endl;
                            cin>>year1>>year2;
                            if(year2<year1 || year2 > max_year || year1 < min_year){
                                cout<<"APPROPRIATELY"<<endl;
                            }
                            else {
                                Get_rcmnds(h,mv_load,temp,year1,year2);
                                break;
                            }
                        }
                        else{
                            Get_rcmnds(h,mv_load,temp,1,1);
                            break;
                        }
                    }
                    break;
                }
                case 3:
                    system("cls");
                    {
                        cout<<"LOADING";
                        blm_fltr temp_bf_1=initialize_data(h);
                        mvc=temp_bf_1;
                        blm_fltr temp_bf_2=create_filter();
                        mv_load=temp_bf_2;
                        while(!gen.empty())
                            gen.pop();
                        while(!rat.empty())
                            rat.pop();
                    }
                    system("cls");
                    cout<<"\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\tAll bloom filters re-initialized and stacks emptied\n\n\t\t\t\t\t\t\t\t\t\tPRESS ANY KEY TO CONTINUE";
                    _getch();
                    break;
                case 4:
                    Show_Credits();
                    break;
                case 5:
                    break;
            }
        }
        else{
            cout<<"\n\t\t\t\t\t\t\t\t\t\tWRONG INPUT DETECTED, PRESS ANY KEY TO CONTINUE";
            _getch();
            system("cls");
        }
    }
    system("cls");
    cout<<"\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\tThank you!!!!!!\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t";
    return 0;
}
