#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Simplex.h"

#define RES	1024
#define CELL_RES 4
#define CELL_SIZE RES / CELL_RES

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace Simplex;

class NoiseGalleryApp : public App {
  public:
	NoiseGalleryApp();
	void update() override;
	void draw() override;
	
	gl::TextureFontRef mFont;
	gl::Texture2dRef mTexture;
	gl::BatchRef mBatch;
};

NoiseGalleryApp::NoiseGalleryApp()
{
	// basic shader that ouput the red channel of the texture
	string vertShader = R"(
		uniform mat4	ciModelViewProjection;
		in vec4			ciPosition;
		in vec2			ciTexCoord0;
		out vec2		vUv;
		void main() {
			vUv = ciTexCoord0;
			gl_Position = ciModelViewProjection * ciPosition;
		}
	)";
	string fragShader = R"(
		uniform sampler2D uSampler;
		in vec2	vUv;
		out vec4 oColor;
		void main()	{
			oColor = vec4( vec3( texture( uSampler, vUv ).r * 0.85f ), 1.0 ); // slightly darker to see the text
		}
	)";
	
	// create a batch and an empty texture
	mBatch = gl::Batch::create( geom::Rect( getWindowBounds() ), gl::GlslProg::create( gl::GlslProg::Format().vertex( vertShader ).fragment( fragShader ) ) );
	mTexture = gl::Texture2d::create( RES, RES, gl::Texture2d::Format().internalFormat( GL_RED ) );
	mFont = gl::TextureFont::create( Font( "Arial", 10 ) );
}

void NoiseGalleryApp::update()
{
	const float time = getElapsedSeconds();
	static GLubyte data[CELL_SIZE*CELL_SIZE];
	for( size_t i = 0; i < CELL_RES * CELL_RES; ++i ) {
		ivec2 offset = ivec2( i % CELL_RES, ( i / CELL_RES ) % CELL_RES ) * ivec2( CELL_SIZE );
		// initialize all noises once and skip non-animated noises after
		if( getElapsedFrames() > 1 ) {
			switch ( i ) {
				case 2: case 7: case 8: case 15: break;
				default: continue; break;
			}
		}
		// update texture cell
		for( size_t x = 0; x < CELL_SIZE; ++x ) {
			for( size_t y = 0; y < CELL_SIZE; ++y ) {
				float n = 0.0f;
				vec2 position = ( vec2( offset ) + vec2( x, y ) ) * 0.01f;
				switch ( i ) {
					case 0: n = noise( position ) * 0.5f + 0.5f; break;
					case 1: n = ridgedNoise( position ); break;
					case 2: n = flowNoise( position, time ) * 0.5f + 0.5f; break;
					case 3: n = fBm( position ) * 0.5f + 0.5f; break;
					case 4: n = fBm( position, 10, 5.0f, 0.75f ) * 0.5f + 0.5f; break;
					case 5: n = fBm( fBm( position * 3.0f ) ) * 0.5f + 0.5f; break;
					case 6: n = fBm( dfBm( position ) ) * 0.5f + 0.5f; break;
					case 7: n = flowNoise( position + fBm( vec3( position, time * 0.1f ) ), time ) * 0.5f + 0.5f; break;
					case 8: n = ridgedMF( vec3( position, time * 0.1f ), 1.0f, 5, 2.0f, 0.65f ); break;
					case 9: n = ridgedMF( position, 0.1f, 5, 1.5f, 1.5f ); break;
					case 10: n = ridgedMF( ridgedMF( position ) ); break;
					case 11: n = ridgedMF( position * 0.25f, -1.0f, 4, 3.0f, -0.65f ); break;
					case 12: n = iqMatfBm( position, 5, mat2( 2.3f, -1.5f, 1.5f, 2.3f ), 0.5f ) * 0.5f + 0.5f; break;
					case 13: n = iqMatfBm( position * 0.75f, 8, mat2( -12.5f, -0.5f, 0.5f, -12.5f ), 0.75f ) * 0.5f + 0.5f; break;
					case 14: n = ( dnoise( position * 5.0f ).y + dnoise( position * 5.0f ).z ) * 0.5f; break;
					case 15: n = noise( position + vec2( curlNoise( position, time ).x ) ) * 0.5f + 0.5f; break;
				}
				data[x+y*CELL_SIZE] = glm::clamp( n, 0.0f, 1.0f ) * 255;
			}
		}
		// upload only the portion of the texture for the current cell
		mTexture->update( data, GL_RED, GL_UNSIGNED_BYTE, 0, CELL_SIZE, CELL_SIZE, ivec2( offset.x, RES - offset.y - CELL_SIZE ) );
	}
}

void NoiseGalleryApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	
	// render the texture
	gl::ScopedTextureBind texBind( mTexture );
	mBatch->draw();
	
	// render titles for each tile
	static string names[16] = { "Noise", "Ridged Noise", "Flow Noise", "fBm", "fBm Variation", "Warped fBm", "dfBm Warped fBm", "fBm Warped Flow Noise", "3D Ridged Multi-Fractal", "Ridged Multi-Fractal Variation", "Warped Ridged Multi-Fractal", "Ridged Multi-Fractal Variation", "iqfBm", "iqfBm Variation", "Noise Derivatives", "Noise Warped Curl Noise" };
	for( size_t i = 0; i < CELL_RES * CELL_RES; ++i ) {
		vec2 offset = vec2( i % CELL_RES, ( i / CELL_RES ) % CELL_RES ) * vec2( CELL_SIZE );
		string name = names[i];
		transform( name.begin(), name.end(), name.begin(), ::toupper );
		mFont->drawString( name, offset + vec2( 5, 15 ) );
	}
}

CINDER_APP( NoiseGalleryApp, RendererGl, []( App::Settings* settings ) {
	settings->setWindowSize( RES, RES );
	settings->setResizable( false );
})
