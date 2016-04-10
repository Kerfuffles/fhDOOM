#pragma once


struct fhUniform {
	enum Value {
		ModelMatrix,
		ViewMatrix,
		ModelViewMatrix,
		ProjectionMatrix,
		LocalLightOrigin,
		LocalViewOrigin,
		LightProjectionMatrixS,
		LightProjectionMatrixT,
		LightProjectionMatrixQ,
		LightFallOff,
		BumpMatrixS,
		BumpMatrixT,
		DiffuseMatrixS,
		DiffuseMatrixT,
		SpecularMatrixS,
		SpecularMatrixT,
		ColorModulate,
		ColorAdd,
		DiffuseColor,
		SpecularColor,
		ShaderParm0,
		ShaderParm1,
		ShaderParm2,
		ShaderParm3,
		TextureMatrix0,
		AlphaTestEnabled,
		AlphaTestThreshold,
		CurrentRenderSize,
		ClipRange,
		DepthBlendMode,
		DepthBlendRange,
		PomMaxHeight,
		Shading,
		specularExp,
		ShadowMappingMode,
		SpotLightProjection,
		PointLightProjection,
		GlobalLightOrigin,
		ShadowParams,
		NUM
	};

	Value value;
};

struct fhRenderProgram {
public:
	static const int vertex_attrib_position = 0;
	static const int vertex_attrib_texcoord = 1;
	static const int vertex_attrib_normal = 2;
	static const int vertex_attrib_color = 3;
	static const int vertex_attrib_binormal = 4;
	static const int vertex_attrib_tangent = 5;
	static const int vertex_attrib_position_shadow = 6;

public:
	fhRenderProgram();
	~fhRenderProgram();

	void Load(const char* vertexShader, const char* fragmentShader);
	void Reload();
	void Purge();
	void Bind(bool force = false) const;	

	const char* vertexShader() const;
	const char* fragmentShader() const;

	static void Unbind();
	static void ReloadAll();
	static void PurgeAll();
	static void Init();

	static void SetModelMatrix(const float* m);
	static void SetViewMatrix(const float* m);
	static void SetModelViewMatrix(const float* m);
	static void SetProjectionMatrix(const float* m);

	static void SetLocalLightOrigin(const idVec4& v);
	static void SetLocalViewOrigin(const idVec4& v);
	static void SetLightProjectionMatrix(const idVec4& s, const idVec4& t, const idVec4& q);
	static void SetLightFallOff(const idVec4& v);
	static void SetBumpMatrix(const idVec4& s, const idVec4& t);
	static void SetDiffuseMatrix(const idVec4& s, const idVec4& t);
	static void SetSpecularMatrix(const idVec4& s, const idVec4& t);
	static void SetTextureMatrix(const float* m);

	static void SetColorModulate(const idVec4& c);
	static void SetColorAdd(const idVec4& c);
	static void SetDiffuseColor(const idVec4& c);
	static void SetSpecularColor(const idVec4& c);

	static void SetShaderParm(int index, const idVec4& v);
	static void SetAlphaTestEnabled(bool enabled);
	static void SetAlphaTestThreshold(float threshold);
	static void SetCurrentRenderSize(const idVec2& uploadSize, const idVec2& viweportSize);
	static void SetClipRange(float nearClip, float farClip);
	static void SetDepthBlendMode(int m);
	static void SetDepthBlendRange(float range);
	static void SetPomMaxHeight(float h);
	static void SetShading(int shading);
	static void SetSpecularExp(float e);

	static void SetShadowMappingMode(int m);
	static void SetSpotLightProjectionMatrix(const float* m);
	static void SetPointLightProjectionMatrices(const float* m);
	static void SetShadowParams(const idVec4& v);
	static void SetGlobalLightOrigin(const idVec4& v);

private:
	static const GLint* currentUniformLocations;
	void Load();

	char   vertexShaderName[64];
	char   fragmentShaderName[64];
	GLuint ident;
	GLint  uniformLocations[fhUniform::NUM];
};

extern const fhRenderProgram* shadowProgram;
extern const fhRenderProgram* interactionProgram;
extern const fhRenderProgram* depthProgram;
extern const fhRenderProgram* shadowmapProgram;
extern const fhRenderProgram* defaultProgram;
extern const fhRenderProgram* skyboxProgram;
extern const fhRenderProgram* bumpyEnvProgram;
extern const fhRenderProgram* fogLightProgram;
extern const fhRenderProgram* vertexColorProgram;
extern const fhRenderProgram* flatColorProgram;
extern const fhRenderProgram* intensityProgram;
extern const fhRenderProgram* blendLightProgram;
extern const fhRenderProgram* depthblendProgram;


ID_INLINE void fhRenderProgram::SetModelMatrix( const float* m ) {
	glUniformMatrix4fv(currentUniformLocations[fhUniform::ModelMatrix], 1, GL_FALSE, m);
}

ID_INLINE void fhRenderProgram::SetViewMatrix( const float* m ) {
	glUniformMatrix4fv(currentUniformLocations[fhUniform::ViewMatrix], 1, GL_FALSE, m);
}

ID_INLINE void fhRenderProgram::SetModelViewMatrix( const float* m ) {
	glUniformMatrix4fv(currentUniformLocations[fhUniform::ModelViewMatrix], 1, GL_FALSE, m);
}

ID_INLINE void fhRenderProgram::SetProjectionMatrix( const float* m ) {
	glUniformMatrix4fv(currentUniformLocations[fhUniform::ProjectionMatrix], 1, GL_FALSE, m);
}

ID_INLINE void fhRenderProgram::SetLocalLightOrigin( const idVec4& v ) {
	glUniform4fv(currentUniformLocations[fhUniform::LocalLightOrigin], 1, v.ToFloatPtr());
}

ID_INLINE void fhRenderProgram::SetLocalViewOrigin( const idVec4& v ) {
	glUniform4fv(currentUniformLocations[fhUniform::LocalViewOrigin], 1, v.ToFloatPtr());
}

ID_INLINE void fhRenderProgram::SetLightProjectionMatrix( const idVec4& s, const idVec4& t, const idVec4& q ) {
	glUniform4fv(currentUniformLocations[fhUniform::LightProjectionMatrixS], 1, s.ToFloatPtr());
	glUniform4fv(currentUniformLocations[fhUniform::LightProjectionMatrixT], 1, t.ToFloatPtr());
	glUniform4fv(currentUniformLocations[fhUniform::LightProjectionMatrixQ], 1, q.ToFloatPtr());
}

ID_INLINE void fhRenderProgram::SetLightFallOff( const idVec4& v ) {
	glUniform4fv(currentUniformLocations[fhUniform::LightFallOff], 1, v.ToFloatPtr());
}

ID_INLINE void fhRenderProgram::SetBumpMatrix( const idVec4& s, const idVec4& t ) {
	glUniform4fv( currentUniformLocations[fhUniform::BumpMatrixS], 1, s.ToFloatPtr() );
	glUniform4fv( currentUniformLocations[fhUniform::BumpMatrixT], 1, t.ToFloatPtr() );
}

ID_INLINE void fhRenderProgram::SetDiffuseMatrix( const idVec4& s, const idVec4& t ) {
	glUniform4fv( currentUniformLocations[fhUniform::DiffuseMatrixS], 1, s.ToFloatPtr() );
	glUniform4fv( currentUniformLocations[fhUniform::DiffuseMatrixT], 1, t.ToFloatPtr() );
}

ID_INLINE void fhRenderProgram::SetSpecularMatrix( const idVec4& s, const idVec4& t ) {
	glUniform4fv( currentUniformLocations[fhUniform::SpecularMatrixS], 1, s.ToFloatPtr() );
	glUniform4fv( currentUniformLocations[fhUniform::SpecularMatrixT], 1, t.ToFloatPtr() );
}

ID_INLINE void fhRenderProgram::SetTextureMatrix( const float* m ) {
	glUniformMatrix4fv(currentUniformLocations[fhUniform::TextureMatrix0], 1, GL_FALSE, m);
}

ID_INLINE void fhRenderProgram::SetColorModulate( const idVec4& c ) {
	glUniform4fv( currentUniformLocations[fhUniform::ColorModulate], 1, c.ToFloatPtr() );
}

ID_INLINE void fhRenderProgram::SetColorAdd( const idVec4& c ) {
	glUniform4fv( currentUniformLocations[fhUniform::ColorAdd], 1, c.ToFloatPtr() );
}

ID_INLINE void fhRenderProgram::SetDiffuseColor( const idVec4& c ) {
	glUniform4fv( currentUniformLocations[fhUniform::DiffuseColor], 1, c.ToFloatPtr() );
}

ID_INLINE void fhRenderProgram::SetSpecularColor( const idVec4& c ) {
	glUniform4fv( currentUniformLocations[fhUniform::SpecularColor], 1, c.ToFloatPtr() );
}

ID_INLINE void fhRenderProgram::SetShaderParm( int index, const idVec4& v ) {
	assert(index >= 0 && index < 4);
	glUniform4fv( currentUniformLocations[fhUniform::ShaderParm0 + index], 1, v.ToFloatPtr() );	
}

ID_INLINE void fhRenderProgram::SetAlphaTestEnabled( bool enabled ) {
	glUniform1i(currentUniformLocations[fhUniform::AlphaTestEnabled], static_cast<int>(enabled));
}

ID_INLINE void fhRenderProgram::SetAlphaTestThreshold( float threshold ) {
	glUniform1f(currentUniformLocations[fhUniform::AlphaTestThreshold], threshold);
}

ID_INLINE void fhRenderProgram::SetCurrentRenderSize( const idVec2& uploadSize, const idVec2& viewportSize ) {
	glUniform4f(currentUniformLocations[fhUniform::CurrentRenderSize], uploadSize.x, uploadSize.y, viewportSize.x, viewportSize.y);
}

ID_INLINE void fhRenderProgram::SetClipRange( float nearClip, float farClip ) {
	glUniform2f(currentUniformLocations[fhUniform::ClipRange], nearClip, farClip);
}

ID_INLINE void fhRenderProgram::SetDepthBlendMode( int m ) {
	glUniform1i(currentUniformLocations[fhUniform::DepthBlendMode], m);
}

ID_INLINE void fhRenderProgram::SetDepthBlendRange( float range ) {
	glUniform1f(currentUniformLocations[fhUniform::DepthBlendRange], range);
}

ID_INLINE void fhRenderProgram::SetPomMaxHeight( float h ) {
	glUniform1f(currentUniformLocations[fhUniform::PomMaxHeight], h);
}

ID_INLINE void fhRenderProgram::SetShading( int shading ) {
	glUniform1i(currentUniformLocations[fhUniform::Shading], shading);
}

ID_INLINE void fhRenderProgram::SetSpecularExp( float e ) {
	glUniform1f(currentUniformLocations[fhUniform::specularExp], e);
}

ID_INLINE void fhRenderProgram::SetShadowMappingMode( int m ) {
	glUniform1i(currentUniformLocations[fhUniform::ShadowMappingMode], m);
}

ID_INLINE void fhRenderProgram::SetSpotLightProjectionMatrix( const float* m ) {
	glUniformMatrix4fv(currentUniformLocations[fhUniform::SpotLightProjection], 1, GL_FALSE, m);
}

ID_INLINE void fhRenderProgram::SetPointLightProjectionMatrices( const float* m ) {
	if(currentUniformLocations[fhUniform::PointLightProjection] != -1) {
		glUniformMatrix4fv(currentUniformLocations[fhUniform::PointLightProjection], 6, GL_FALSE, m);
	}
}

ID_INLINE void fhRenderProgram::SetShadowParams( const idVec4& v ) {
	glUniform4fv(currentUniformLocations[fhUniform::ShadowParams], 1, v.ToFloatPtr());
}

ID_INLINE void fhRenderProgram::SetGlobalLightOrigin( const idVec4& v ) {
	glUniform4fv(currentUniformLocations[fhUniform::GlobalLightOrigin], 1, v.ToFloatPtr());
}