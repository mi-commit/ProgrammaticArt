using UnityEngine;

public class hands : MonoBehaviour
{
    Animator animator => GetComponent<Animator>();
    // Start is called once before the first execution of Update after the MonoBehaviour is created

    float duration = 5;
    private void OnEnable()
    {
        animator.SetTrigger("HandsTrigger");
        duration = 5.5f;
    }
    private void Update()
    {
        if (duration < 0) gameObject.SetActive(false);
        duration -= Time.deltaTime;
    }


}
