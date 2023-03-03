#include <locale>
#include <string>
#include <sys/stat.h>
#include <windows.h>



char rus1[]="йцукенгшщзхъфывапролджэячсмитьбюё";
char rus2[]="ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮЁ";
char lat1[]="y_c_u_k_e_n_g_shscz_h___f_y_v_a_p_r_o_l_d_zhe_yachs_m_i_t___b_yuyo";
char lat2[]="Y_C_U_K_E_N_G_SHSCZ_H___F_Y_V_A_P_R_O_L_D_ZHE_YACHS_M_I_T___B_YUYO";

long len_tables = sizeof(rus1);

void preinit(void)
{
    if (sizeof(rus1) != sizeof(rus2) )
    {
          wprintf(L"call programmer\n");
        exit(10);
    }
    if (sizeof(rus1) != sizeof(lat1) )
    {
          wprintf(L"call programmer\n");
        exit(10);
    }
    if (sizeof(rus1) != sizeof(lat2) )
    {
          wprintf(L"call programmer\n");
        exit(10);
    }
    for (int i = 0; i< len_tables; i++)
    {
        if (lat1[i] == '_')
            lat1[i] = 0;
        if (lat2[i] == '_')
            lat2[i] = 0;
    }
}

long mod2bytes(char* arr2)
{
    long res = 0;
    for (int i = 0; i< len_tables; i+=2)
    {
        if (arr2[0] == rus1[i])
            if (arr2[1] == rus1[i+1])
            {
                arr2[0] = lat1[i];
                arr2[1] = lat1[i+1];
                res = 1;
                break;
            }


        if (arr2[0] == rus2[i])
            if (arr2[1] == rus2[i+1])
            {
                arr2[0] = lat2[i];
                arr2[1] = lat2[i+1];
                res = 1;
                break;
            }

    }
    return res;
}


using namespace std;


class TFileStream
{
public:
    FILE* filedesc;// = _wfopen(FileName.c_str(),L"rb"); // https://stackoverflow.com/questions/29187070/reading-from-a-binary-file-in-c/29187280
    long writeprotect;
    wstring last_filename;

    void Init(void)
    {
        filedesc = NULL;
        writeprotect = 1;
    }
    long OpenRead(wstring filename)
    {
        try
        {
            filedesc = _wfopen(filename.c_str(),L"rbS"); // https://stackoverflow.com/questions/29187070/reading-from-a-binary-file-in-c/29187280
            if (filedesc == NULL) return 0;
            last_filename = filename;
        }
        catch (...)
        {
            return 0;
        }
        writeprotect = 1;
        return 1;
    }
    long CreateFile4Write(wstring filename)
    {
        try
        {
            filedesc = _wfopen(filename.c_str(),L"wbS");
            if (filedesc == NULL) return 0;
            last_filename = filename;
            writeprotect = 0;
        }
        catch (...)
        {
            return 0;
        }
        return 1;
    }
    TFileStream* OnflyCreateFile4Write(wstring filename)
    {
        long rez = CreateFile4Write(filename);
        if (rez)
            return this;
        else
            return NULL;
    }
    long Read(unsigned char* Buff_bytes,long len)
    {
        size_t len2 = fread(Buff_bytes, 1, len, filedesc);
        return len2;
    }
    long long Size(void)
    {
        long long size2=0;
        struct __stat64 st;
        if (_wstat64(last_filename.c_str(),&st)==0)
            size2 = st.st_size;
        if (size2 <= 0) return 0;
        return size2;
    }
    long Write(unsigned char* Buff_bytes,long len)
    {
        if (writeprotect) return -1;
        size_t len2 = fwrite(Buff_bytes, 1, len, filedesc);
        return len2;
    }
    long WriteStr(string str)
    {
        const char* Buff_bytes = str.c_str();
        long len = str.size();
        if (writeprotect) return -1;
        size_t len2 = fwrite(Buff_bytes, 1, len, filedesc);
        return len2;
    }
    long WriteStrNTerm(char* chs)
    {
        const char* Buff_bytes = chs;
        long len = 0;
        while (chs[len]!=0) len++;
        if (writeprotect) return -1;
        size_t len2 = fwrite(Buff_bytes, 1, len, filedesc);
        return len2;
    }
    void Close(void)
    {
        fclose(filedesc);
    }
};


int wmain(int argc, wchar_t *argv[])
{
    setlocale(LC_ALL, "Russian");

    preinit();

    if (argc < 2)
    {
        wprintf(L">translite  FILE_NAME_TXT_UTF8\n");
        return 1;
    }

    TFileStream myfile;
    wstring fn(argv[1]);
    wprintf(L"file %ls\n",fn.c_str());
    if (myfile.OpenRead(fn))
    {
        long long len = myfile.Size();
        wprintf(L"file size %i\n",len);
        unsigned char* mem = (unsigned char*)malloc(len+10);
        memset(mem,0,len+10);
        myfile.Read(mem,len);
        myfile.Close();
        if (len > 1)
        {
            for (long long i = 0; i< len-1; i++)
            {
                if (mem[i] & 0x80)
                {
                    if (mod2bytes((char*)&(mem[i]))) i++;
                }
            }

            long long tx = 0;
            for (long long rx = 0; rx< len-1; rx++)
            {
                if (mem[rx] != 0)
                {
                    mem[tx] = mem[rx];
                    tx++;
                }
            }
            mem[tx]=0;
            myfile.CreateFile4Write(fn);
            myfile.Write(mem,tx);
            myfile.Close();

        }
    }
    else
    {
        wprintf(L"Error2\n");
        wprintf(L"translite  FILE_NAME_TXT_UTF8\n");
        return 2;
    }



    return 0;
}
