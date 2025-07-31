Shader "Graph/Point surface GPU"
{
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 200

        CGPROGRAM
        // Physically based Standard lighting model, and enable shadows on all light types
        #pragma surface ConfigureSurface Standard FullForwardShadows addshadow
        #pragma editor_sync_compilation
        // // Use shader model 3.0 target, to get nicer looking lighting
        #pragma instancing_options procedural:ConfigureProcedural assumeuniformscaling 

        #pragma target 4.5

        struct Input{
            float3 worldPos;
         };
         float _Step;
         #if defined (UNITY_PROCEDURAL_INSTANCING_ENABLED)
         StructuredBuffer<float3> _Positions;
         #endif
         void ConfigureProcedural(){
            #if defined (UNITY_PROCEDURAL_INSTANCING_ENABLED)
            float3 position = _Positions[unity_InstanceID];
            unity_ObjectToWorld = 0.0;
            unity_ObjectToWorld._m03_m13_m23_m33 = float4(position, 1.0);
            unity_ObjectToWorld._m00_m11_m22 = _Step;

            #endif


         }
         void ConfigureSurface(Input input, inout SurfaceOutputStandard surface){
			// surface.Albedo.rb = saturate(input.worldPos.y * (0.51) - 0.49);
            surface.Albedo.rgb = saturate(input.worldPos.xyz * (0.5) + 0.5);

            // surface.Smoothness = saturate(input.worldPos.y);
         }
        ENDCG
    }
    FallBack "Diffuse"
}
