#version 400

in vec3 normal;
in vec2 texture_coordinates;

in vec3 n_eye; // normals from vertex shader
in vec3 p_eye;
in mat4 view_mat;
in mat4 proj_mat;

uniform sampler2D _texture;

uniform float move_spotlight_x;
uniform float move_spotlight_z;

uniform mat4 Light_Mov; // light translation matrix

out vec4 frag_colour;

vec3 light_pos_world = vec3 (0.0, 8.0, 0.0);

vec3 l_dir = normalize (vec3 (-1.0f, -1.0f, -1.0f));
vec3 l_a = vec3 (0.2, 0.2, 0.2);	// Grey ambient light
vec3 l_d = vec3 (0.94, 0.87, 0.47);	// Light yellow diffuse light
vec3 l_s = vec3 (1.0, 1.0, 1.0);	// White specular light

float spec_exp = 200.0f;	// Specular "power"

vec3 k_a = vec3 (1.0, 1.0, 1.0);	// Fully reflect ambient light
vec3 k_d = vec3 (0.8, 0.8, 0.8);	// Slightly dimly reflect diffuse light
vec3 k_s = vec3 (0.1, 0.1, 0.1);	// Dim specular reflection

void main() {

	vec3 norm_eye = normalize (n_eye);

	vec3 light_position_offset = vec3(move_spotlight_x, 0.0, move_spotlight_z);
	vec3 light_position_world = light_pos_world + light_position_offset;
	//vec3 light_position_world = vec3(view_mat*proj_mat*Light_Mov*vec4(light_pos_world, 1.0));
	
	vec4 texel = texture (_texture, texture_coordinates);

	vec3 light_position_eye = vec3 (view_mat * vec4 (light_position_world, 1.0));
	//vec3 light_position_eye = light_position_world;
	vec3 distance_to_light_eye = light_position_eye - p_eye;
	vec3 direction_to_light_eye = normalize (distance_to_light_eye);

	/* Spotlight */
	vec3 spot_direction = normalize (vec3 (0.0, -1.0, 0.0));
	const float spot_arc = 1.0 - 5.0 / 90.0;
	float spot_dot = dot (spot_direction, -direction_to_light_eye);

	float spot_factor = (spot_dot - spot_arc) / (1.0 - spot_arc);
	spot_factor = clamp (spot_factor, 0.0, 1.0);

	// diffuse intensity
	float dot_prod = dot (direction_to_light_eye, norm_eye);
	dot_prod = max (dot_prod, 0.0);
	vec3 Id = l_d * texel.xyz * dot_prod; // final diffuse intensity
	Id *= spot_factor;

	// specular intensity
	vec3 surface_to_viewer_eye = normalize (-p_eye);

	// blinn
	vec3 half_way_eye = normalize (surface_to_viewer_eye + direction_to_light_eye);
	float dot_prod_specular = max (dot (half_way_eye, norm_eye), 0.0);
	float specular_factor = pow (dot_prod_specular, spec_exp);

	vec3 Is = l_s * k_s * specular_factor; // final specular intensity
	Is *= spot_factor;

	// Ambient light (only in spotlight area)
	vec3 Ia = l_a * texel.xyz;
	Ia *= spot_factor;

	
	// Ambient light (none)
	//vec3 Ia = vec3 (0.0, 0.0, 0.0);

	vec3 I = vec3 (0.0, 0.0, 0.0);
	I += Ia;
	I += Id;
	//I += Is;
	
	frag_colour = vec4 (I, 1.0);
}
