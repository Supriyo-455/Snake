#include "Texture.h"

Texture::Texture(SDL_Renderer* renderer)
{
    m_Renderer = renderer;
    m_Texture = NULL;
    m_Width = 0;
    m_Height = 0;
}

Texture::~Texture()
{
    // Deallocate
    free();
}

bool Texture::loadFromFile(std::string path)
{
    // Get rid of preexisitng texture
    free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(m_Renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			m_Width = loadedSurface->w;
			m_Height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	m_Texture = newTexture;
	return m_Texture != NULL;
}

bool Texture::loadFromRenderedText(std::string textureText, TTF_Font* font, Color color)
{
	// Get rid of preexisitng texture
	free();

	SDL_Color texSDLColor = {};
	texSDLColor.r = color.r;
	texSDLColor.g = color.g;
	texSDLColor.b = color.b;
	texSDLColor.a = color.a;

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), texSDLColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
    else
    {
        //Create texture from surface pixels
        m_Texture = SDL_CreateTextureFromSurface(m_Renderer, textSurface);
        if (m_Texture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get image dimensions
            m_Width = textSurface->w;
            m_Height = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    //Return success
    return m_Texture != NULL;
}

void Texture::free()
{
	//Free texture if it exists
	if (m_Texture != NULL)
	{
		SDL_DestroyTexture(m_Texture);
		m_Texture = NULL;
		m_Width = 0;
		m_Height = 0;
	}
}

void Texture::setColor(Color color)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(m_Texture, color.r, color.g, color.b);
}

void Texture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(m_Texture, blending);
}

void Texture::setAlpha(uint8_t alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(m_Texture, alpha);
}

void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, m_Width, m_Height };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(m_Renderer, m_Texture, clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth()
{
	return m_Width;
}

int Texture::getHeight()
{
	return m_Height;
}