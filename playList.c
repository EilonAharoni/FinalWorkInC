
#include <stdlib.h>
#include "string.h"
#include "playList.h"
#include "General.h"
#include "fileHelper.h"
#include "macros.h"



int initPlayListForSystem(PlayList* pPlay)

{
    pPlay->numOfSongs=0;
    pPlay->playlistName = getStrExactName("Enter PlayList Name");
    CHECK_RETURN_0(pPlay->playlistName)
    pPlay->allSongs= NULL;
    pPlay->playListSortOp = eNotOrdered;
    pPlay->typeOfPlayList = eSystem;
    return 1;
}

int initPlayListForUser(PlayList* pPlay)
{
    pPlay->numOfSongs=0;
    pPlay->playlistName = getStrExactName("Enter PlayList Name");
    CHECK_RETURN_0(pPlay->playlistName)
    pPlay->allSongs= NULL;
    pPlay->playListSortOp = eNotOrdered;
    pPlay->typeOfPlayList = eUser;
    return 1;
}

int addSongToPlayList(PlayList* pPlay, Song* pSong)
{
    CHECK_RETURN_0(pSong)
    CHECK_RETURN_0(pPlay)
    if (pPlay->numOfSongs > 1)
    {
        if(getSongFromPlayList(pPlay,pSong->songCode))
        {
            printf("Song all ready in PlayList\n");
            return DUPLICATE;
        }
    }
    pPlay->allSongs = (Song**)realloc(pPlay->allSongs, (pPlay->numOfSongs + 1) * sizeof(Song*));
    CHECK_RETURN_0(pPlay->allSongs)
    pPlay->allSongs[pPlay->numOfSongs] = pSong;
    pPlay->numOfSongs++;
    pPlay->playListSortOp = eNotOrdered; // when we add a song the arr becomes not sorted.
    return 1;
}

Song* getSongFromPlayList(const PlayList* pPlay,const char code[5])
{
    CHECK_RETURN_0(pPlay)
    if (pPlay->numOfSongs < 1)
    {
        printf("PlayList is empty\n");
        return NULL;
    }
    Song* temp;
    for (int i = 0; i < pPlay->numOfSongs; i++) {
        temp = pPlay->allSongs[i];
        if(!strcmp(temp->songCode,code))
            return temp;
    }
    return NULL;
 }

int removeSongFromPlayList(PlayList* pPlay)
{
    CHECK_RETURN_0(pPlay)
    if (pPlay->numOfSongs < 1)
    {
        printf("PlayList is empty\n");
        return NOT_ENOUGH;
    }
    printf("Enter index of song to remove\n");
    printPlayList(pPlay);
    int index;
    do{
        printf("Enter index 1-%d\n",pPlay->numOfSongs);
        scanf("%d",&index);
    }
    while(index<0||index>pPlay->numOfSongs);
    index -=1; // index goes from 1..numOfSongs,  so now 0...numOfSongs-1.
    pPlay->allSongs[index] = pPlay->allSongs[pPlay->numOfSongs-1];
    pPlay->allSongs = (Song**)realloc(pPlay->allSongs, (pPlay->numOfSongs - 1) * sizeof(Song*));
    pPlay->numOfSongs--;
    if(pPlay->numOfSongs != 0) // if PlayList isnt empty
        if(!pPlay->allSongs)
            return ERROR;
    pPlay->playListSortOp = eNotOrdered; // when we remove a song the arr becomes not sorted.
    return 1;
}

ePlayListsType playListTypeMenu()
{
    int opt;
    printf("PlayList Options:\n");
    do {
        for (int i = 0; i < eNofPlayListType; i++)
            printf("Enter %d for %s\n", i, playListType[i]);
        scanf("%d", &opt);
    } while (opt < 0 || opt >=eNofPlayListType);

    return (ePlayListsType)opt;
}


void sortPlayList(PlayList* pPlay)
{
    CHECK_RETURN(pPlay)
    if (pPlay->numOfSongs < 1)
    {
        printf("PlayList is empty\n");
        return;
    }
    pPlay->playListSortOp = showSortMenu();
    int(*compare) (const void* S1,const void* S2) = NULL;
    switch(pPlay->playListSortOp)
    {
        case eAmountPlayed:
            compare = compareByAmountPlayed;
            break;
        case eArtistName:
            compare = compareByArtistName;
            break;
        case eGenre:
            compare = compareByGenre;
            break;
        case eSongName:
            compare = compareByName;
            break;
    }
    if (compare != NULL)
    {
        qsort(pPlay->allSongs, pPlay->numOfSongs, sizeof(Song*), compare);
        printf("PlayList Sorted.\n");
    }
}

void findSong(const PlayList* pPlay) 
{
    CHECK_RETURN(pPlay)
    if (pPlay->numOfSongs < 1)
    {
        printf("PlayList is empty\n");
        return;
    }
    int(*compare) (const void* S1,const void* S2) = NULL;
    Song sTemp = {0};
    Artist temp1 = {0};
    Song* pTemp = &sTemp;
    char temp[MAX_STR_LEN];
    switch(pPlay->playListSortOp)
    {
        case eAmountPlayed:
            compare = compareByAmountPlayed;
            printf("Enter the amount to search for\n");
            scanf("%d",&sTemp.amountPlayedSong);
            break;
        case eArtistName:
            compare = compareByArtistName;
            printf("Enter Artist name to search for\n");
            myGets(temp,MAX_STR_LEN,stdin);
            sTemp.artist.name = temp;
            break;
        case eGenre:
            compare = compareByGenre;
            sTemp.typeOfSong = genreMenu();
            break;
        case eSongName:
            compare = compareByName;
            printf("Enter Song name to search for\n");
            myGets(temp,MAX_STR_LEN,stdin);
            sTemp.songName = temp;
            break;
        case eNotOrdered:
            printf("Array not sorted, cant perform search.\n");
            break;
    }
    if (compare != NULL) {
        Song** pS = bsearch(&pTemp, pPlay->allSongs, pPlay->numOfSongs, sizeof(Song*), compare);
        if (pS == NULL) {
            printf("didnt find\n");
        }
        else {
            printSong(*pS);
        }
    }
}


eSortOption showSortMenu()
{
    int opt;
    printf("Base on what field do you want to sort?\n");
    do {
        for (int i = 1; i < eNofSortOpt; i++)
            printf("Enter %d for %s\n", i, sortOptStr[i]);
        scanf("%d", &opt);
    } while (opt < 0 || opt >=eNofSortOpt);

    return (eSortOption)opt;
}


void printPlayList(const PlayList* pPlay) {
    CHECK_RETURN(pPlay)
    if (pPlay->numOfSongs < 1)
    {
        printf("Playlist name: %s is empty\n",pPlay->playlistName);
        return;
    }
    printf("PlayList (%s) Name: %s, Has %d Songs\n",playListType[pPlay->typeOfPlayList],pPlay->playlistName,pPlay->numOfSongs);
    generalArrayFunctionForRepostiory(pPlay->allSongs, pPlay->numOfSongs, sizeof(Song*), printSongForPlayListptr);
    printf("\n");
}

void freePlayList(PlayList* pPlay)
{
    free(pPlay->playlistName);
    free(pPlay->allSongs);
}
void freeSongsArr(Song** pSongs,int size)
{
    generalArrayFunction(pSongs, size, sizeof(Song*), free);
}

int createSongArr(PlayList* pPlay) // for reading from a file
{
    if(pPlay->numOfSongs>0)
    {
        pPlay->allSongs = (Song**) malloc(pPlay->numOfSongs*sizeof(Song*));
        CHECK_RETURN_0_PRINT(pPlay->allSongs, ALOC_ERROR)
    }
    else
        pPlay->allSongs = NULL;

    return 1;
}

int writePlayListToBFile(const PlayList* pPlay,FILE* fp)
{
    CHECK_RETURN_0(pPlay)
    if(!writeStringToFile(pPlay->playlistName,fp,"Error Writing PlayList Name"))
        return 0;
    if(!writeIntToFile(pPlay->numOfSongs,fp,"Error Writing number of songs"))
        return 0;
    for (int i = 0; i < pPlay->numOfSongs; i++) {
        if(!writeCharsToFile(pPlay->allSongs[i]->songCode,5,fp,"Error Writing Song code"))
            return 0;
    }
    if(!writeIntToFile(pPlay->typeOfPlayList,fp,"Error Writing PlayList Type"))
        return 0;
    return 1;

}
int writePlayListToTextFile(const PlayList* pPlay,FILE* fp)
{
    CHECK_RETURN_0(pPlay)
    fprintf(fp,"%s\n",pPlay->playlistName);
    fprintf(fp,"%d\n",pPlay->numOfSongs);
    for (int i = 0; i < pPlay->numOfSongs; i++) {
        fprintf(fp,"%s\n",pPlay->allSongs[i]->songCode);
    }
    fprintf(fp,"%d\n",pPlay->typeOfPlayList);
    return 1;
}
int readPlayListFromBFile(PlayList* pPlay, FILE* fp,const SongRepository* sR)
{
    CHECK_RETURN_0(pPlay)
        pPlay->playlistName = readStringFromFile(fp, "Error Reading PlayList Name");
    if (!pPlay->playlistName)
        return 0;
    if (!readIntFromFile(&pPlay->numOfSongs, fp, "Error Reading number of songs"))
    {
        free(pPlay->playlistName);
        return 0;
    }
    if(!createSongArr(pPlay))
    {
        free(pPlay->playlistName);
        return 0;
    }
    char temp[CODE_LENGTH];
    for (int i = 0; i < pPlay->numOfSongs; i++) {
        if(!readCharsFromFile(temp, CODE_LENGTH,fp,"Error Reading Song Code"))
        {
            free(pPlay->playlistName);
            free(pPlay->allSongs);
            return 0;
        }
        pPlay->allSongs[i] = getSongFromRepositoryByCode(sR,temp);
        CHECK_RETURN_0(pPlay->allSongs[i])
    }
    int temp2;
    if(!readIntFromFile(&temp2,fp,"Error Reading Playlist Type"))
    {
        free(pPlay->playlistName);
        free(pPlay->allSongs);
        return 0;
    }
    pPlay->typeOfPlayList = temp2;
    pPlay->playListSortOp = eNotOrdered;
    return 1;
}
int readPlayListFromTextFile(PlayList* pPlay,FILE* fp,const SongRepository* sR)
{
    char temp2[MAX_STR_LEN];
    if(!pPlay)
        return 0;
    myGets(temp2,MAX_STR_LEN,fp);
    pPlay->playlistName = getDynStr(temp2);
    CHECK_RETURN_0(pPlay->playlistName)
    if(fscanf(fp,"%d",&pPlay->numOfSongs) != 1)
    {
        free(pPlay->playlistName);
        return 0;
    }
    char temp[CODE_LENGTH];
    Song* s;
    if(!createSongArr(pPlay))
    {
        free(pPlay->playlistName);
        return 0;
    }
    for (int i = 0; i < pPlay->numOfSongs; i++) {
        myGets(temp, CODE_LENGTH,fp);
        s = getSongFromRepositoryByCode(sR,temp);
        if(!s)
        {
            free(pPlay->playlistName);
            free(pPlay->allSongs);
            return 0;
        }
        pPlay->allSongs[i] = s;
    }
    if(fscanf(fp,"%d",&pPlay->typeOfPlayList) != 1)
    {
        free(pPlay->playlistName);
        free(pPlay->allSongs);
        return 0;
    }
    pPlay->playListSortOp = eNotOrdered;
    return 1;
}