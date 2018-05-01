// Cinematographic black effect for cutscenes.

static Campaign_DimShader;

// Actually no fading but instead blackness
global func DimPlayers()
{
	Campaign_DimShader = AddFragmentShader("Common", "slice(finish+20) { fragColor = vec4(vec3(0.0*fragColor.r + 0.0*fragColor.g + 0.0*fragColor.b), fragColor.a); }");
}

global func UndimPlayers()
{
	RemoveShader(Campaign_DimShader);
}