
#include <stdio.h>
#include <stdlib.h>
#include "artistRepository.h"
#include "macros.h"
#include "General.h"


int initArtistRepositoryFromFile(ArtistRepository* artistRepository,const char* fileName, int typeFile)
{
    FILE* fp;
    if (typeFile == FROM_TEXT_FILE)// init from text file
    {
        fp = fopen(fileName, "r");
        CHECK_RETURN_0_PRINT(fp,"Error open artist file\n")
            if (fscanf(fp, "%d", &artistRepository->numOfArtist) != 1) {
                fclose(fp);
                return 0;
            }

        if(!createArtistArr(artistRepository))//printing allocation error
        {
            fclose(fp);
            return 0;
        }

        for (int i = 0; i < artistRepository->numOfArtist; ++i)
        {
            if(!loadArtistFromTextFile(&artistRepository->allArtists[i],fp))
            {
                for (int j = 0; j < i; ++j)
                {
                    freeArtist(&artistRepository->allArtists[j]);
                }
                free(artistRepository->allArtists);
                return 0;
            }
        }
        fclose(fp);
        return 1;

    }

    if (typeFile == FROM_BINARY_FILE)
    {

            fp = fopen(fileName, "rb");
            CHECK_RETURN_0(fp)
            BYTE dataCounter;
            if (fread(&dataCounter, sizeof(BYTE),1,fp) != 1)
            {
                fclose(fp);
                return 0;
            }
            artistRepository->numOfArtist = dataCounter;

            if(!createArtistArr(artistRepository))//printing allocation error
            {
                fclose(fp);
                return 0;
            }

            for (int i = 0; i < artistRepository->numOfArtist; ++i)
            {
                if(!readArtistFromCompressFile(&artistRepository->allArtists[i],fp))
                {
                    for (int j = 0; j < i; ++j)
                    {
                        freeArtist(&artistRepository->allArtists[i]);

                    }
                    free(artistRepository->allArtists);
                    return 0;
                }

            }
        fclose(fp);
        return 1;
   }
    else //invalid input
        return 0;
}

int saveArtistRepositoryToFile(ArtistRepository* artistRepository, const char* fileName, int typeFile)
{
    FILE* fp;
    if (typeFile == FROM_TEXT_FILE)// init from text file
    {
        fp = fopen(fileName, "w");
        CHECK_RETURN_0_PRINT(fp,"Error open artist repository file to write\n")

       if(!fprintf(fp, "%d\n", artistRepository->numOfArtist))
       {
           fclose(fp);
           return 0;
       }
        for (int i = 0; i <artistRepository->numOfArtist ; ++i)
        {
           if(!saveArtistToTextFile(&artistRepository->allArtists[i],fp))
               return 0;
        }
        fclose(fp);
        return 1;
    }
    if (typeFile == FROM_BINARY_FILE)
    {
        if (artistRepository->numOfArtist > MAX_ARTISTS_NUM)
        PRINT_RETURN_0("Cant compress artists to File")// num of artist more than we can compress

        fp = fopen(fileName, "wb");
        CHECK_RETURN_0_PRINT(fp,"Error open artist repository file to write\n")

        BYTE bCounter = (BYTE)artistRepository->numOfArtist;
        if (fwrite(&bCounter, sizeof(BYTE),1,fp) != 1)
        {
            fclose(fp);
            return 0;
        }
        for (int i = 0; i < artistRepository->numOfArtist; ++i)
        {
            if(!saveArtistToCompressFile(&artistRepository->allArtists[i],fp))
            {
                fclose(fp);
                return 0;
            }
        }
        fclose(fp);
        return 1;

    }else
        return 0;
}


int showArtistRepository(const ArtistRepository* artistRepository)
{
    if (!artistRepository || artistRepository->numOfArtist<0)
        return 0;
    generalArrayFunctionForRepostiory(artistRepository->allArtists,artistRepository->numOfArtist, sizeof(Artist),showArtist);


    return 1;
}

int	createArtistArr(ArtistRepository* artistRepository)
{
    artistRepository->allArtists = (Artist *)malloc(artistRepository->numOfArtist * sizeof(Artist));
    if (!artistRepository->allArtists)
    {
        printf("Allocation error\n");
        return 0;
    }
    return 1;
}

void freeArtistRepository(ArtistRepository * artistRepository)
{
    CHECK_RETURN(artistRepository->allArtists)// if not initialize don't free
//    if (songRepository == NULL)
//        return;

    for (int i = 0; i < artistRepository->numOfArtist; ++i)
    {
        freeArtist(&artistRepository->allArtists[i]);
    }
    free(artistRepository->allArtists);
}

int addArtistToRepository(ArtistRepository* artistRepository)
{
    artistRepository->allArtists = (Artist *)realloc(artistRepository->allArtists,(artistRepository->numOfArtist + 1)*sizeof(Artist));
    if(!artistRepository->allArtists)
        return ERROR;
   if(!creatArtist(&artistRepository->allArtists[artistRepository->numOfArtist++]))
       return ERROR;
//    artistRepository->allArtists[artistRepository->numOfArtist++] = *pSong;
    return 1;

}