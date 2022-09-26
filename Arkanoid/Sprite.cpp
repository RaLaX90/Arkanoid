#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::Reset()
{
}

void Sprite::Initialize(ID2D1HwndRenderTarget* m_pRenderTarget)
{
}

//void Sprite::Move(int mouseX, int mouseY, double elapsedTime)
//{
//}

void Sprite::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
}

Point2D Sprite::GetPosition()
{
	return position;
}

void Sprite::SetWidth(int w)
{
	width = w;
}

int Sprite::GetWidth()
{
	return width;
}

void Sprite::SetHeight(int h)
{
	height = h;
}

int Sprite::GetHeight()
{
	return height;
}

ID2D1Bitmap* Sprite::setBackgroundImage(ID2D1HwndRenderTarget* m_pRenderTarget, LPCWSTR imageFile)
{
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICImagingFactory* pIWICFactory = NULL;

	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(&pIWICFactory));

	HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
		imageFile,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	ID2D1Bitmap* toReturn = NULL;

	if (SUCCEEDED(hr))
	{

		// Create a Direct2D bitmap from the WIC bitmap.
		hr = m_pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&toReturn
		);
	}

	SafeRelease(&pIWICFactory);
	SafeRelease(&pConverter);
	SafeRelease(&pSource);
	SafeRelease(&pDecoder);

	return toReturn;
}