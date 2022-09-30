using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Photon.Pun;
using UnityEngineInternal;
using System.Transactions;

public class PlayerController : MonoBehaviourPunCallbacks
{
    public Transform viewpoint;
    public float mouseSensitivity = 1f;
    private float verticalRotStore;
    private Vector2 mouseInput;

    public bool invertLook;

    public float moveSpeed = 5f , runSpeed = 8f;
    private float activeMoveSpeed;
    private Vector3 moveDir, movement;

    public CharacterController characterController;

    private Camera camera;

    public float jumpForce = 12f , gravityMod = 2.5f;

    public Transform groundCheckPoint;
    private bool isGrounded;
    public LayerMask groundLayers;

    public GameObject builletImpact;
    // public float timeBetweenShots = .1f;
    private float shotCounter;

    public float maxHeat = 10f, /*heatPerShot = 1f, */ coolRate = 4, overheatCoolRate = 5f;
    private float heatCounter;
    private bool overHeated;

    public Gun[] allGuns;
    private int selectedGun;
    public float muzzleDisplayTime;
    private float muzzleCounter;

    public GameObject playerHitImpact;

    public int maxHealth = 100;
    private int currentHealth;


    // Start is called before the first frame update
    void Start()
    {
        Cursor.lockState = CursorLockMode.Locked;

        camera = Camera.main;

        UIController.instance.weaponTempSlider.maxValue = maxHeat;
        
        SwitchGun();
        currentHealth = maxHealth;
        UIController.instance.healthSlier.maxValue = maxHealth;
        UIController.instance.healthSlier.value = currentHealth;

        /*
        Transform newTrans = SpawnManager.instance.GetSpawnPoint();
        transform.position = newTrans.position;
        transform.rotation = newTrans.rotation;
        */
    }

    // Update is called once per frame
    void Update()
    {
        if (photonView.IsMine)
        {

            // �¿� ī�޶� �̵�(�÷��̾ �̵�)
            mouseInput = new Vector2(Input.GetAxisRaw("Mouse X"), Input.GetAxisRaw("Mouse Y")) * mouseSensitivity;
            transform.rotation = Quaternion.Euler(transform.rotation.eulerAngles.x, transform.rotation.eulerAngles.y + mouseInput.x, transform.rotation.eulerAngles.z);

            // ���� ī�޶� �̵�(ī�޶� �̵�)
            verticalRotStore += mouseInput.y;
            verticalRotStore = Mathf.Clamp(verticalRotStore, -60f, 60f);

            if (invertLook)
            {
                viewpoint.rotation = Quaternion.Euler(verticalRotStore, viewpoint.rotation.eulerAngles.y, viewpoint.rotation.eulerAngles.z);
            }
            else
            {
                viewpoint.rotation = Quaternion.Euler(-verticalRotStore, viewpoint.rotation.eulerAngles.y, viewpoint.rotation.eulerAngles.z);
            }
            float yVal = movement.y;
            // ĳ���Ͱ� �ٶ󺸴� �������� �̵�
            moveDir = new Vector3(Input.GetAxisRaw("Horizontal"), 0f, Input.GetAxisRaw("Vertical"));
            movement = (transform.forward * moveDir.z + transform.right * moveDir.x).normalized * activeMoveSpeed;


            // ĳ���� �ӵ� (�� shift)
            if (Input.GetKey(KeyCode.LeftShift)) activeMoveSpeed = runSpeed;
            else activeMoveSpeed = moveSpeed;
            // �߷� �߰�
            movement.y = yVal;
            if (characterController.isGrounded) movement.y = 0f;
            // ĳ���� ����
            isGrounded = Physics.Raycast(groundCheckPoint.position, Vector3.down, .25f, groundLayers);
            if (Input.GetButtonDown("Jump") && isGrounded) movement.y = jumpForce;
            movement.y += Physics.gravity.y * Time.deltaTime * gravityMod;
            characterController.Move(movement * Time.deltaTime);
            if (allGuns[selectedGun].muzzleFlash.activeInHierarchy)
            {
                muzzleCounter -= Time.deltaTime;
                if (muzzleCounter <= 0)
                {
                    allGuns[selectedGun].muzzleFlash.SetActive(false);
                }
            }// ���콺 Ŭ���� �� �߻�
            if (!overHeated) // ���� �ȉ�ٸ�
            {
                if (Input.GetMouseButtonDown(0))
                {
                    Shoot();
                }
                // ���콺 ������������ ����
                if (Input.GetMouseButton(0) && allGuns[selectedGun].isAutomatic)
                {
                    shotCounter -= Time.deltaTime;
                    if (shotCounter <= 0)
                    {
                        Shoot();
                    }
                }
                heatCounter -= Time.deltaTime;
            }
            else // ������ٸ�
            {
                heatCounter -= overheatCoolRate * Time.deltaTime;
                if (heatCounter <= 0)
                {
                    overHeated = false;

                    UIController.instance.overheatedMessage.gameObject.SetActive(false);
                }
            }
            if (heatCounter < 0)
            {
                heatCounter = 0f;
            }
            UIController.instance.weaponTempSlider.value = heatCounter;
            // �� ���� ��ũ��
            if (Input.GetAxisRaw("Mouse ScrollWheel") > 0f)
            {
                selectedGun++;
                selectedGun %= allGuns.Length;
                SwitchGun();
            }
            else if (Input.GetAxisRaw("Mouse ScrollWheel") < 0f)
            {
                selectedGun--;
                if (selectedGun < 0)
                    selectedGun = allGuns.Length - 1;
                SwitchGun();
            }
            for (int i = 0; i < allGuns.Length; i++)
            {
                if (Input.GetKeyDown((i + 1).ToString()))
                {
                    selectedGun = i;
                    SwitchGun();
                }
            }



            // ���콺 lock / unlock
            if (Input.GetKeyDown(KeyCode.Escape)) Cursor.lockState = CursorLockMode.None;
            else if (Cursor.lockState == CursorLockMode.None)
            {
                if (Input.GetMouseButtonDown(0)) Cursor.lockState = CursorLockMode.Locked;
            }
        }
    }

    private void Shoot()
    {
        Ray ray = camera.ViewportPointToRay(new Vector3(.5f, .5f, 0f));
        ray.origin = camera.transform.position;

        if(Physics.Raycast(ray,out RaycastHit hit))
        {
            //Debug.Log("We hit " + hit.collider.gameObject.name);

            if (hit.collider.gameObject.tag == "Player")
            {
                Debug.Log("hit " + hit.collider.gameObject.GetPhotonView().Owner.NickName);
                PhotonNetwork.Instantiate(playerHitImpact.name, hit.point, Quaternion.identity);

                hit.collider.gameObject.GetPhotonView().RPC("DealDamage",RpcTarget.All,photonView.Owner.NickName, allGuns[selectedGun].shotDamage);
            }
            else
            {


                GameObject bulletImapctObject = Instantiate(builletImpact, hit.point + (hit.normal * .002f), Quaternion.LookRotation(hit.normal, Vector3.up));

                Destroy(bulletImapctObject, 10f);
            }
        }

        shotCounter = allGuns[selectedGun].timeBetweenShots;

        heatCounter += allGuns[selectedGun].heatPershot;
        if(heatCounter >= maxHeat)
        {
            heatCounter = maxHeat;
            overHeated = true;
            
            UIController.instance.overheatedMessage.gameObject.SetActive(true);
            
        }

        allGuns[selectedGun].muzzleFlash.SetActive(true);
        muzzleCounter = muzzleDisplayTime;
    }

    [PunRPC]
    public void DealDamage(string damager,int damageAmount)
    {
        TakeDamage(damager, damageAmount);
    }
    public void TakeDamage(string damager,int damageAmount)
    {
        if (photonView.IsMine)
        {
            currentHealth -= damageAmount;
            UIController.instance.healthSlier.value = currentHealth;
            if (currentHealth <= 0)
            {

                PlayerSpawner.Instance.Die(damager);
            }
        }
    }
    // update �Լ� ���� �� lateupdate�Լ��� ����ȴ�
    private void LateUpdate()
    {
        if(photonView.IsMine)
        {
            // ī�޶� �̵�
            camera.transform.position = viewpoint.position;
            camera.transform.rotation = viewpoint.rotation;
        }
    }
    void SwitchGun()
    {
        foreach(Gun gun in allGuns)
        {
            gun.gameObject.SetActive(false);
            allGuns[selectedGun].muzzleFlash.SetActive(false);
        }
        allGuns[selectedGun].gameObject.SetActive(true);


    }

}
