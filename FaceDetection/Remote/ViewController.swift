//
//  ViewController.swift
//  Remote
//
//  Created by Jagdish Katariya on 15/03/19.
//  Copyright © 2019 Jagdish Katariya. All rights reserved.
//

import UIKit
import Firebase
import FirebaseDatabase
let mySegmentedFan=UISegmentedControl (items: ["OFF Fan","ON Fan"])
let mySegmentedLight=UISegmentedControl (items: ["OFF Light","ON Light"])
class ViewController: UIViewController {
    
    var ref: DatabaseReference!
    @IBOutlet weak var counterPeople: UILabel!
    override func viewDidLoad() {
        super.viewDidLoad()
        
        let xPostion:CGFloat = 10
        let yPostion:CGFloat = 150
        let elementWidth:CGFloat = 300
        let elementHeight:CGFloat = 30
        
        mySegmentedFan.frame = CGRect(x: xPostion, y: yPostion, width: elementWidth, height: elementHeight)
        mySegmentedFan.selectedSegmentIndex = 1
        mySegmentedFan.tintColor = UIColor(red:0.13, green:0.55, blue:0.13, alpha:1.0)
        mySegmentedFan.backgroundColor = UIColor.white
        mySegmentedFan.addTarget(self, action: #selector(ViewController.fan_status(_:)), for: .valueChanged)
        self.view.addSubview(mySegmentedFan)
        let xPostion1:CGFloat = 10
        let yPostion1:CGFloat = 200
        let elementWidth1:CGFloat = 300
        let elementHeight1:CGFloat = 30
        mySegmentedLight.frame = CGRect(x: xPostion1, y: yPostion1, width: elementWidth1, height: elementHeight1)
        mySegmentedLight.selectedSegmentIndex = 1
        mySegmentedLight.tintColor = UIColor(red:0.13, green:0.55, blue:0.13, alpha:1.0)
        mySegmentedLight.backgroundColor = UIColor.white
        mySegmentedLight.addTarget(self, action: #selector(ViewController.light_status(_:)), for: .valueChanged)
        self.view.addSubview(mySegmentedLight)
        ref = Database.database().reference(withPath: "Home_status/device")
        _ = ref.observe(DataEventType.value, with: { (snapshot) in
            let postDict = snapshot.value as? [String : AnyObject] ?? [:]
            if(postDict["light"]! as! Int==0){
                mySegmentedLight.selectedSegmentIndex = 0
                mySegmentedLight.tintColor = UIColor(red:0.56, green:0.74, blue:0.56, alpha:1.0)
            }else if(postDict["light"]! as! Int==1){
                mySegmentedLight.selectedSegmentIndex = 1
                mySegmentedLight.tintColor = UIColor(red:0.13, green:0.55, blue:0.13, alpha:1.0)
            }
            if(postDict["fan"]! as! Int==0){
                mySegmentedFan.selectedSegmentIndex = 0
                mySegmentedFan.tintColor = UIColor(red:0.56, green:0.74, blue:0.56, alpha:1.0)
            }else if(postDict["fan"]! as! Int==1){
                mySegmentedFan.selectedSegmentIndex = 1
                mySegmentedFan.tintColor = UIColor(red:0.13, green:0.55, blue:0.13, alpha:1.0)
            }
        })
        ref = Database.database().reference(withPath: "counter")
        _ = ref.observe(DataEventType.value, with: { (snapshot) in
            print(snapshot.value!)
            let val=snapshot.value!
            self.counterPeople.text="\(val)"
//            self.counterPeople.text=snapshot.value! as? String
            self.counterPeople.textColor=UIColor.black
        })
    }
    @objc func fan_status(_ sender:UISegmentedControl!)
    {
        if sender.selectedSegmentIndex==0 {
            self.ref.updateChildValues(["fan": false])
        }else if sender.selectedSegmentIndex==1{
            self.ref.updateChildValues(["fan": true])
        }
    }
    @objc func light_status(_ sender:UISegmentedControl!)
    {
        if sender.selectedSegmentIndex==0 {
            self.ref.updateChildValues(["light": false])
        }else if sender.selectedSegmentIndex==1{
            self.ref.updateChildValues(["light": true])
        }
    }
}
