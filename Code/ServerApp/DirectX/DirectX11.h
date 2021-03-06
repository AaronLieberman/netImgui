#pragma once

#include <NetImgui_Api.h>

#include <vector>

namespace NetImgui { namespace Internal { struct CmdDrawFrame;  struct CmdTexture; } }

namespace dx
{

struct TextureHandle
{
							TextureHandle(): mImguiId(0), mIndex(0), mTextureFormat(NetImgui::eTexFormat::kTexFmt_Invalid){}	
	inline bool				IsValid(){ return mTextureFormat != NetImgui::eTexFormat::kTexFmt_Invalid; }
	inline void				SetInvalid(){ mTextureFormat = NetImgui::eTexFormat::kTexFmt_Invalid; }
	uint64_t				mImguiId;
	uint32_t				mIndex;
	NetImgui::eTexFormat	mTextureFormat;
};

bool			Startup(HWND hWindow);
void			Shutdown();
void			Render(const std::vector<TextureHandle>& textures, const NetImgui::Internal::CmdDrawFrame* pDrawFrame);

TextureHandle	TextureCreate(NetImgui::Internal::CmdTexture* pCmdTexture);
void			TextureRelease(TextureHandle& hTexture);

}
