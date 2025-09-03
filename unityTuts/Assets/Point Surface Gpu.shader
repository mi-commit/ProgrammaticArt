Shader "Graph/Point surface GPU"
{
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 200

        CGPROGRAM
        // configureSurface means having a surface kernel, Fullforwarshadows are shadow things and addshadow means we add a custom shadow pass
        #pragma surface ConfigureSurface Standard FullForwardShadows addshadow
        #pragma editor_sync_compilation 
        //make sure this specific shader is always compiled ahead of time, as the dummy shader could cause issues due to procedural drawing
        #pragma instancing_options procedural:ConfigureProcedural assumeuniformscaling 

        #pragma target 4.5 //for compute shader & buffer support

        float _Step;

        struct Input{
            float3 worldPos;
         };

         
         #if defined (UNITY_PROCEDURAL_INSTANCING_ENABLED)
         StructuredBuffer<float3> _Positions;
         #endif


         void ConfigureProcedural(){
            #if defined (UNITY_PROCEDURAL_INSTANCING_ENABLED)
            float3 position = _Positions[unity_InstanceID];

            unity_ObjectToWorld = 0.0; //initialize object to world conversion matrix
            //this transformation will then be applied for each vertex i believe
            //im notating down what the matrix should look like at each step just so i understand it
            //for object position offset = p and step = s 
            // 0   0   0   0
            // 0   0   0   0
            // 0   0   0   0
            // 0   0   0   0

            unity_ObjectToWorld._m03_m13_m23_m33 = float4(position, 1.0);
            //0   0   0   p   
            //0   0   0   p   
            //0   0   0   p   
            //0   0   0   p   

            unity_ObjectToWorld._m00_m11_m22 = _Step;
            //unity_ObjectToWorld._m11= _Step*2.;
            //s   0   0   p   
            //0   s   0   p   
            //0   0   s   p   
            //0   0   0   p
            #endif

         }
         void ConfigureSurface(Input input, inout SurfaceOutputStandard surface){
            //same surface things as before 

			surface.Albedo.rb = saturate(-input.worldPos.y * (0.51));
            //surface.Albedo.rgb = saturate(input.worldPos.xyz * (0.5) + 0.5);

            // surface.Smoothness = saturate(input.worldPos.y);
         }
        ENDCG
    }
    FallBack "Diffuse"
}
