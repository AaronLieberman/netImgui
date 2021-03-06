#pragma once

#include "NetImgui_Shared.h"

//=============================================================================
// Forward Declares
//=============================================================================
namespace NetImgui { namespace Internal { struct CmdTexture; struct CmdDrawFrame; struct CmdInput; } }
namespace NetImgui { namespace Internal { namespace Network { struct SocketInfo; } } }

namespace NetImgui { namespace Internal { namespace Client
{

//=============================================================================
// Keep a list of textures used by Imgui, needed by server
//=============================================================================
struct ClientTexture
{
	inline void	Set( CmdTexture* pCmdTexture );
	inline bool	IsValid()const;	
	CmdTexture* mpCmdTexture= nullptr;
	bool		mbSent		= false;
	uint8_t		mPadding[7]	= {0};
};

//=============================================================================
// Keep all Client infos needed for communication with server
//=============================================================================
struct ClientInfo
{
	using VecTexture	= ImVector<ClientTexture>;
	using BufferKeys	= Ringbuffer<uint16_t, 1024>;
	using Time			= std::chrono::time_point<std::chrono::high_resolution_clock>;

								ClientInfo();
	Network::SocketInfo*		mpSocket					= nullptr;
	char						mName[16]					={0};
	VecTexture					mTextures;
	CmdTexture*					mTexturesPending[16];
	ExchangePtr<CmdDrawFrame>	mPendingFrameOut;
	ExchangePtr<CmdInput>		mPendingInputIn;
	BufferKeys					mPendingKeyIn;
	ImGuiContext*				mpContextClone				= nullptr;	// Default ImGui drawing context copy, used to do our internal remote drawing
	ImGuiContext*				mpContextEmpty				= nullptr;	// Placeholder ImGui drawing context, when we are not waiting for a new drawing frame but still want a valid context in place
	ImGuiContext*				mpContextRestore			= nullptr;	// Context to restore to Imgui once drawing is done
	Time						mTimeTracking;
	std::atomic_int32_t			mTexturesPendingCount;
	float						mMouseWheelVertPrev			= 0.f;
	float						mMouseWheelHorizPrev		= 0.f;
	int							mRestoreKeyMap[ImGuiKey_COUNT];
	ImGuiConfigFlags			mRestoreConfigFlags			= 0;
	const char*					mRestoreBackendPlatformName	= nullptr;
	const char*					mRestoreBackendRendererName	= nullptr;	
	ImGuiBackendFlags			mRestoreBackendFlags		= 0;
	bool						mbConnected					= false;	// Successfully Connected
	bool						mbDisconnectRequest			= false;	// Waiting to Disconnect
	bool						mbConnectRequest			= false;	// Waiting to Connect	
	bool						mbHasTextureUpdate			= false;
	bool						mbIsDrawing					= false;	// True if we are inside a NetImgui::NewFrame() / NetImgui::EndFrame() call
	bool						mbIsRemoteDrawing			= false;	// True if the rendering it meant for the remote netImgui server
	bool						mbRestorePending			= false;	// Original context has had some settings overridden, original values stored in mRestoreXXX
	char						PADDING[5];
	void						TextureProcessPending();
	void						TextureProcessRemoval();

// Prevent warnings about implicitly created copy
protected:
	ClientInfo(const ClientInfo&)=delete;
	ClientInfo(const ClientInfo&&)=delete;
	void operator=(const ClientInfo&)=delete;
};

//=============================================================================
// Main communication thread, that should be started in its own thread
//=============================================================================
void CommunicationsClient(void* pClientVoid);
void CommunicationsHost(void* pClientVoid);

}}} //namespace NetImgui::Internal::Client

#include "NetImgui_Client.inl"
