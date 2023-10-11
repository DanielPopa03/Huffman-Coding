#include <stdlib.h>
#include <stdio.h>
typedef struct dict
{
    char nr_car;
    unsigned char caracter;
    int nr_aparitii;
    struct dict *st;
    struct dict *dr;
} dict;
typedef struct aj
{
    int x;
    unsigned char cod[256];
} aj;
int main()
{
    long int size_dictionar,size_fis;
    unsigned char c[256];
    aj v[256];
    int nr_car_prez_in_fisier = 0;
    for (int i = 0; i < 256; i++)
        v[i].x = 0;
    FILE *f = fopen("fisier.in", "r");
    if (f == NULL)
    {
        printf("Fisierul nu a putut fi deschis\n");
        return 1;
    }
    for (int c = getc(f); c != EOF; c = getc(f))
    {
        if (v[c].x == 0) 
           nr_car_prez_in_fisier += 1;
        v[c].x += 1;
    }
    /*for(int i=0;i<256;i++){
        if(v[i].x!=0)
         nr_car_prez_in_fisier += 1;
    }*/
    printf("%d \n",nr_car_prez_in_fisier);
    size_fis=ftell(f);
    //nr_car_prez_in_fisier += 10;
    fclose(f);
    f=fopen("dict.bin","wb");
    dict *nou;
    dict *root = NULL;
    dict **aux = (dict **)calloc(nr_car_prez_in_fisier, sizeof(dict *));
    for (int i = 0; i < nr_car_prez_in_fisier; i++)
        (aux)[i] = (dict *)calloc(nr_car_prez_in_fisier, sizeof(dict));
    for (int i = 0, j = 0; i < 256; i++)
    {  int a;
        if (v[i].x != 0)
        {   a=v[i].x;
            fwrite(&i,sizeof(unsigned char),1,f);
            if(size_fis>10000)
            fwrite(&a,sizeof(int),1,f);
            else
            fwrite(&a,sizeof(unsigned char),1,f);
            (aux)[j]->nr_car = 1;
            //(aux)[j]->caracter=(unsigned char *)malloc(sizeof(unsigned char));
            (aux)[j]->caracter = i;
            (aux)[j]->nr_aparitii = v[i].x;
            j++;
        }
    }
    size_dictionar=ftell(f);
    fclose(f);
    int k = nr_car_prez_in_fisier;
    if (k == 1 || k == 0)
        return 0;
    while (k > 2)
    {
        mergeSort(aux, 0, k - 1);
        /*for(int i=0;i<k;i++)
       printf("%d ",(aux)[i]->nr_aparitii);
       printf("\n");*/
        nou = (dict *)calloc(1,sizeof(dict));
        nou->st = *(aux + k - 2);
        nou->dr = *(aux + k - 1);
        nou->nr_aparitii = nou->st->nr_aparitii + (nou->dr)->nr_aparitii;
        nou->nr_car = (nou->st)->nr_car + (nou->dr)->nr_car;
        *(aux + k - 2) = nou;
        k -= 1;
        aux = (dict **)realloc(aux, k * sizeof(dict));
    }

    mergeSort(aux, 0, k - 1);
    root = (dict *)calloc(nr_car_prez_in_fisier, sizeof(dict));
    root->st = *(aux + k - 2);
    root->dr = *(aux + k - 1);
    A(root->st, 0, '0', v, c);
    A(root->dr, 0, '1', v, c);
    // Afisare(root);
    /*for (int i = 0; i < 256; i++)
    {
        if (v[i].x != 0)
            printf("%c %s %d\n", i, v[i].cod, v[i].x);
    }*/

    f = fopen("fisier.in", "r");
    FILE *f1 = fopen("bin.bin", "wb");
    if (f == NULL)
    {
        printf("Fisierul nu a putut fi deschis\n");
        return 1;
    }
    unsigned char buffer = 0;
    long int size;
    for (char c = getc(f), poz = 0; c != EOF; c = getc(f))
    {
        for (int i = 0; v[c].cod[i] != '\0'; i++)
        {
            unsigned char c1 = v[c].cod[i];
            // printf("%c",c1,c);
            buffer |= (c1 - '0') << (7 - poz);
            ++poz;
            if (poz == 8)
            {
                poz = 0;
                // printf(" %u ",buffer);
                fwrite(&buffer, sizeof(buffer), 1, f1);
                buffer = 0;
            }
        }
    }
    fwrite(&buffer, sizeof(buffer), 1, f1);
    fclose(f);
    fclose(f1);
    f1 = fopen("bin.bin", "rb");
    f = fopen("re.in", "w+");
    fseek(f1, 0, SEEK_END);
    size = ftell(f1);
    fseek(f1, 0, SEEK_SET);
    char c1;
    dict *l=root;
    while (size != ftell(f1))
    {
        fread(&buffer, sizeof(buffer), 1, f1);
        //printf("%d ",buffer);
        for (int poz = 7; poz >= 0; poz--)
        {
            if(l->st==NULL) {
                fprintf(f, "%c", l->caracter);
                //printf(" %c ",l->caracter[0]);
                l=root;
            }
            c1 = (buffer & (1 <<  (poz))) >> ( poz);
            //printf(" %d ",c1);
            if(c1==0) l=l->st;
            if(c1==1) l=l->dr;
        }
    }
    free(root);
    printf("Rata conversiei este de %f",(float) ((size+size_dictionar)*100)/size_fis);
    return 0;
}
void merge(dict **arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    dict L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = *(arr[l + i]);
    for (j = 0; j < n2; j++)
        R[j] = *(arr[m + 1 + j]);
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i].nr_aparitii >= R[j].nr_aparitii)
        {
            *(arr[k]) = L[i];
            i++;
        }
        else
        {
            *(arr[k]) = R[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        *(arr[k]) = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        *(arr[k]) = R[j];
        j++;
        k++;
    }
}
void mergeSort(dict **arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}
void A(dict *aux, int i, char o, aj *v, char c[])
{
    c[i] = o;
    if (aux->st != NULL)
    {
        A(aux->st, i + 1, '0', v, c);
        A(aux->dr, i + 1, '1', v, c);
    }
    else
    {
        unsigned char ceva = aux->caracter;
        for (int j = 0; j <= i; j++)
        {
            v[ceva].cod[j] = c[j];
        }
        v[ceva].cod[i + 1] = '\0';
    }
}
void Afisare(dict *aux)
{
    if (aux != NULL)
    {
        // if(aux->st!=NULL)
         //printf("%c ",aux->caracter,aux->nr_aparitii);
        //printf("%d %c\n", aux->nr_aparitii, aux->nr_car);
        Afisare(aux->st);
        Afisare(aux->dr);
    }
}
void free_tree(dict * node){

       if (node != NULL) {
        free_tree(node->dr);
        free_tree(node->st);
        free(node);
     }}