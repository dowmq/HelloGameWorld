#include "SpriteFactory.h"

Sprite* SpriteFactory::CreateSprite(std::string text_filename, char comment_char,
	int index, int width, int interval)
{
	std::unique_ptr<std::vector<std::vector<std::string>>> ParseResult;
	try
	{
		ParseResult = TextFileParser::GetSerialString(text_filename, comment_char);
		std::vector<std::string> firstLine = ParseResult->front();
		ParseResult->erase(ParseResult->begin());

		std::string bitmap_name = firstLine[0];
		std::wstring fName(bitmap_name.begin(), bitmap_name.end());
		// 파일에 젤 처음에 적혀 있는 비트맵 파일 경로를 읽고 오픈함
		HANDLE hFile = CreateFile(fName.c_str(), GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			throw std::logic_error("Bitmap File doesn't exist!!" + bitmap_name);
		}

		BITMAPFILEHEADER m_bitFileHeader;
		DWORD dwRead;
		ReadFile(hFile, &m_bitFileHeader,
			sizeof(BITMAPFILEHEADER), &dwRead, NULL);

		DWORD dwDataSize;
		dwDataSize = m_bitFileHeader.bfOffBits -
			sizeof(BITMAPFILEHEADER);
		BITMAPINFO* bitInfo = (BITMAPINFO*)malloc(dwDataSize);
		ReadFile(hFile, bitInfo,
			dwDataSize, &dwRead, NULL);

		PVOID pRaster = nullptr;
		HDC ScreenDC = CWindow::Instance().GetScreenDC();

		HBITMAP t_hBitmap = CreateDIBSection(
			ScreenDC, bitInfo, DIB_RGB_COLORS, &pRaster, NULL, 0);
		ReadFile(hFile, pRaster,
			m_bitFileHeader.bfSize - m_bitFileHeader.bfOffBits,
			&dwRead, NULL);
		HDC memDC = CreateCompatibleDC(ScreenDC);

		// 읽은 정보를 기반으로 CBitmap 객체를 만들고 Sprite에 넣는다.
		Sprite* new_sprite = new Sprite;
		new_sprite->text_file_name = text_filename;
		for (auto eachElement : *ParseResult.get())
		{
			CBitmap* element = new CBitmap;
			element->offScreenDC = CWindow::Instance().GetOffScreenDC();
			element->m_hBitmap = t_hBitmap;
			element->m_hMemDC = memDC;//CreateCompatibleDC(ScreenDC);
			//element->m_hMemDC = CreateCompatibleDC(ScreenDC);
			element->bitmap_rect.left = std::atoi(eachElement[0].c_str()) + (index * (interval + width));
			element->bitmap_rect.top = std::atoi(eachElement[1].c_str());
			element->bitmap_rect.right = std::atoi(eachElement[2].c_str());
			element->bitmap_rect.bottom = std::atoi(eachElement[3].c_str());
			SelectObject(element->m_hMemDC, element->m_hBitmap);
			GetObject(element->m_hBitmap, sizeof(BITMAP), &element->m_BitmapInfo);

			new_sprite->imageList.push_back(element);
		}
		new_sprite->number_of_frame = new_sprite->imageList.size();
		new_sprite->timer = &TimeHandler::Instance();
		new_sprite->is_dead_sprite = false;
		if (firstLine.size() > 1) {
			new_sprite->is_loop_sprite = firstLine[1] == "true" ? true : false;
			new_sprite->life_time = std::atoi(firstLine[2].c_str());
			new_sprite->image_change_time = std::atof(firstLine[3].c_str());
		}
		else {
			new_sprite->is_loop_sprite = true;
			new_sprite->life_time = 5000;
			new_sprite->image_change_time = 100.0;
		}

		new_sprite->elapsed_time = 0.0f;
		new_sprite->elapsed_time_per_frame = 0.0f;
		new_sprite->current_frame = 0;

		free(bitInfo);
		CloseHandle(hFile);

		return new_sprite;
	}
	catch (OpenFailException& e)
	{
		std::string str = "SpriteFactory File Open Fail -";
		str.append(e.what());
		std::wstring wstr(str.begin(), str.end());
		MessageBox(CWindow::Instance().m_hWnd, wstr.c_str(), TEXT("Exception"), MB_ICONERROR);
	}
	catch (std::exception& e)
	{
		std::string str(e.what());
		std::wstring wstr(str.begin(), str.end());
		MessageBox(CWindow::Instance().m_hWnd, wstr.c_str(), TEXT("Exception"), MB_ICONERROR);
	}
	return nullptr;
}

std::vector<Sprite*>* SpriteFactory::CreateSprites(std::string text_filename, size_t number_of_sprites, char comment_char)
{
	std::unique_ptr<std::vector<std::vector<std::string>>> ParseResult;
	try
	{
		ParseResult = TextFileParser::GetSerialString(text_filename, '/');
		std::vector<std::string> firstLine = ParseResult->front();
		ParseResult->erase(ParseResult->begin());

		std::string bitmap_name = firstLine[0];
		std::wstring fName(bitmap_name.begin(), bitmap_name.end());
		// 파일에 젤 처음에 적혀 있는 비트맵 파일 경로를 읽고 오픈함
		HANDLE hFile = CreateFile(fName.c_str(), GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			throw std::logic_error("Bitmap File doesn't exist!!" + bitmap_name);
		}

		BITMAPFILEHEADER m_bitFileHeader;
		DWORD dwRead;
		ReadFile(hFile, &m_bitFileHeader,
			sizeof(BITMAPFILEHEADER), &dwRead, NULL);

		DWORD dwDataSize;
		dwDataSize = m_bitFileHeader.bfOffBits -
			sizeof(BITMAPFILEHEADER);
		BITMAPINFO* bitInfo = (BITMAPINFO*)malloc(dwDataSize);
		ReadFile(hFile, bitInfo,
			dwDataSize, &dwRead, NULL);

		PVOID pRaster = nullptr;
		HDC ScreenDC = CWindow::Instance().GetScreenDC();

		HBITMAP t_hBitmap = CreateDIBSection(
			ScreenDC, bitInfo, DIB_RGB_COLORS, &pRaster, NULL, 0);
		ReadFile(hFile, pRaster,
			m_bitFileHeader.bfSize - m_bitFileHeader.bfOffBits,
			&dwRead, NULL);
		HDC memDC = CreateCompatibleDC(ScreenDC);

		std::vector<Sprite*>* result = new std::vector<Sprite*>();
		// 읽은 정보를 기반으로 CBitmap 객체륾 만들고 Sprite에 넣는다.
		for (auto eachElement : *ParseResult.get())
		{
			for (size_t i = 0; i < number_of_sprites; ++i)
			{
				Sprite* new_sprite = new Sprite;
				new_sprite->text_file_name = text_filename;
				CBitmap* element = new CBitmap;
				element->offScreenDC = CWindow::Instance().GetOffScreenDC();
				element->m_hBitmap = t_hBitmap;
				element->m_hMemDC = memDC;//CreateCompatibleDC(ScreenDC);
				element->bitmap_rect.left = 
					std::atoi(eachElement[0].c_str()) 
					+ (i * ( std::atoi(eachElement[4].c_str()) + std::atoi(eachElement[2].c_str()))  );
				new_sprite->nIndex = i;
				new_sprite->width = std::atoi(eachElement[2].c_str());
				new_sprite->interval = (std::atoi(eachElement[4].c_str()));
				element->bitmap_rect.top = std::atoi(eachElement[1].c_str());
				element->bitmap_rect.right = std::atoi(eachElement[2].c_str());
				element->bitmap_rect.bottom = std::atoi(eachElement[3].c_str());
				SelectObject(element->m_hMemDC, element->m_hBitmap);
				GetObject(element->m_hBitmap, sizeof(BITMAP), &element->m_BitmapInfo);

				new_sprite->imageList.push_back(element);
				new_sprite->number_of_frame = new_sprite->imageList.size();
				new_sprite->timer = &TimeHandler::Instance();
				new_sprite->is_dead_sprite = false;
				new_sprite->is_loop_sprite = true;
				new_sprite->life_time = 0;
				new_sprite->image_change_time = 1000.0;
				new_sprite->elapsed_time = 0.0f;
				new_sprite->elapsed_time_per_frame = 0.0f;
				new_sprite->current_frame = 0;
				result->push_back(new_sprite);
			}

		}
		free(bitInfo);
		CloseHandle(hFile);

		return result;
	}
	catch (OpenFailException& e)
	{
		std::string str = "SpriteFactory File Open Fail -";
		str.append(e.what());
		std::wstring wstr(str.begin(), str.end());
		MessageBox(CWindow::Instance().m_hWnd, wstr.c_str(), TEXT("Exception"), MB_ICONERROR);
	}
	catch (std::exception& e)
	{
		std::string str(e.what());
		std::wstring wstr(str.begin(), str.end());
		MessageBox(CWindow::Instance().m_hWnd, wstr.c_str(), TEXT("Exception"), MB_ICONERROR);
	}
	return nullptr;
}