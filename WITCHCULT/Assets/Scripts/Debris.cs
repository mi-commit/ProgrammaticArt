using UnityEngine;

[RequireComponent(typeof(MeshFilter), typeof(MeshRenderer))]
public class Debris : MonoBehaviour
{
    float _lifetime;
    Mesh _mesh;
    Material _material;
    [SerializeField] DebrisData d;
    [SerializeField] static float speed = .5f;

    public void Init(DebrisData data, float lifetime, Vector3 position)
    {
        d = data;
        _lifetime = lifetime;
        transform.position = position;
        transform.localScale = Vector3.one * .5f;
        GetComponent<MeshFilter>().mesh = data.mesh;
        GetComponent<MeshRenderer>().material = data.material;
    }

    void Update()
    {
        _lifetime -= Time.deltaTime;
        if (_lifetime < 0) Destroy(gameObject);
        gameObject.transform.position += Vector3.up * speed * Time.deltaTime;
    }
}
