//---------------------------------------------
//                Initial Engine
//                Texture Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Video/ITextureVideo.h"
#include "Video/ITextureManager.h"

using namespace Initial;

ITextureVideo::ITextureVideo(ITextureManager *manager, IString filename)
: ITexture(manager,filename)
{		
#if USE_BINK
	BinkSoundUseDirectSound( 0 );

	Bink = BinkOpen( filename, 0 );

	if (Bink)
	{
		int width=2;
		int height=2;
		while (width<Bink->Width) width*=2;
		while (height<Bink->Height) height*=2;
		m_pBuffer.SetImage(width,height);
		printf("Video loaded\n");
	}else
		printf("Video loading error\n");
#endif
}

ITextureVideo::~ITextureVideo()
{
#if USE_BINK
	if (Bink)
		BinkClose( Bink );
#endif
}

void ITextureVideo::UpdateImage()
{
#if USE_BINK
	if (Bink /*&& !BinkWait( Bink )*/)
	{
		BinkDoFrame( Bink );

		BinkCopyToBuffer( Bink,
						  m_pBuffer.GetData(),
						  m_pBuffer.GetFormat()*m_pBuffer.GetWidth(),
						  Bink->Height,
						  0,0,
						  BINKSURFACE24R );

		if (m_pTextureManager)
			m_pTextureManager->SetTextureData(this,m_pBuffer);
		//printf("Process bink\n");
		BinkNextFrame( Bink );
	}
#endif
}

void ITextureVideo::Play()
{
	if (Bink);
}

void ITextureVideo::Pause()
{
	if (Bink);	
}

void ITextureVideo::Stop()
{
	if (Bink);
}

void ITextureVideo::SetPos(float time)
{
	/*if (Bink)
		BinkGoto(Bink,1+Bink->FrameRate()*time);*/
}

float ITextureVideo::GetLength()
{
	if (Bink)
		return Bink->Frames/(float)Bink->FrameRate;

	return 0;
}

float ITextureVideo::GetPos()
{
	if (Bink)
		return Bink->FrameNum;

	return 0;
}
