//
//  HistoryTableViewController.swift
//  Remote
//
//  Created by Jagdish Katariya on 20/03/19.
//  Copyright © 2019 Jagdish Katariya. All rights reserved.
//
class Calls {
    var mydate: String?
    var mytime: String?
    var myfan: String?
    var mylight: String?
    
    init(mydate: String?, mytime: String?, myfan: String?, mylight: String?) {
        self.mydate = mydate
        self.mytime = mytime
        self.myfan = myfan
        self.mylight = mylight
    }
}

//class HeadlineTableViewCell : UITableViewCell{

//    @IBOutlet weak var timeLable: UILabel!
    
//}
import UIKit
import FirebaseDatabase

//var refreshControl: UIRefreshControl!
class HistoryTableViewController: UITableViewController{
    
    @IBOutlet weak var timeLable: UILabel!
    @IBOutlet weak var status1Lable: UILabel!
    var dict = Dictionary<String, Array<String>>()
    var device1 = [String : String]()
    var time1 =  [String : [String : String]] ()
    var date1 = [String : [String : [String : String]]] ()
    var new_data = [String]()
    var new_time = [String]()
    var number_row = [Int]()
    var final_data =  [String : [String : String]] ()

    let activityIndicator = UIActivityIndicatorView(style: UIActivityIndicatorView.Style.whiteLarge)

    var expandedSectionHeaderNumber: Int = 0
//    var expandedSectionHeader: UITableViewHeaderFooterView!
//    var sectionItems: Array<Any> = []
//    var sectionNames: Array<Any> = []
    
    @IBOutlet var tableviewdata: UITableView!
    var ref: DatabaseReference!
    var myCallList = [Calls]()
    override func viewDidLoad() {
        super.viewDidLoad()
//        activityIndicator.center = CGPoint(x: 40, y: 50)
        activityIndicator.center = CGPoint(x:UIScreen.main.bounds.size.width / 2, y:UIScreen.main.bounds.size.height / 2)
        activityIndicator.color = UIColor.blue
        self.view.addSubview(activityIndicator)
        activityIndicator.startAnimating()

        // this is the replacement of implementing: "collectionView.addSubview(refreshControl)"
//        UICollectionView.refreshControl = refreshControl


//        dict1["fan"] = []
        ref = Database.database().reference(withPath: "Home_logs")
         self.navigationItem.rightBarButtonItem = self.editButtonItem
        ref.observe(DataEventType.value, with: { (snapshot) in
//            print("================Change===============")
//            if let userName = snapshot.value!["19-03-20"] as? String {
//                print(userName)
//            }
            
            var date="" as String
            var fan2="Nil"
            var light2="Nil"
            var time=""
//            var i=0
            for snap in snapshot.children
            {
                let userSnap = snap as! DataSnapshot
                 date=userSnap.key //the uid of each user
                self.time1.removeAll()
//                print(self.time1)
                let userDict = userSnap.value as! [String:AnyObject]
                for data in userDict{
                    fan2="Nil"
                    light2="Nil"
                    time=data.key
                    let values=data.value
                    let fan=values["fan"]!
                    let light=values["light"]!
                    if fan != nil{
                        if let fan1=fan{
                            if fan1 as! Bool == true{
                                fan2="ON"
                            }else{
                                fan2="OFF"
                            }
                        }
                    }
                    if light != nil{
                        if let light1=light{
                            if light1 as! Bool == true{
                                light2="ON"
                            }else{
                                light2="OFF"
                            }
                        }
                    }
                    self.device1["fan"] = fan2
                    self.device1["light"] = light2
                    self.time1[time] = self.device1
                }
                self.date1[date] = self.time1

            }
            self.new_data = self.date1.keys.sorted().reversed()
            for index_name in self.new_data{
                let data=self.date1[index_name]
                self.number_row.append(data!.count)
            }
            self.activityIndicator.stopAnimating()
            self.tableviewdata.reloadData()
      
        })

    
    }
    @objc func doSomething(refreshControl: UIRefreshControl) {
        refreshControl.endRefreshing()
    }
    func add(string:String,key:String) {
        
        if var value = dict[key] {
            
            // if an array exist, append to it
            
            value.append(string)
            
            dict[key] = value
            
        } else {
            
            // create a new array since there is nothing here
            
            dict[key] = [string]
        }
    }

    // MARK: - Table view data source

    override func numberOfSections(in tableView: UITableView) -> Int {
        // #warning Incomplete implementation, return the number of sections
            return self.date1.count
    }
    

    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        // #warning Incomplete implementation, return the number of rows
        if (self.expandedSectionHeaderNumber == section) {
            print("number_row ",number_row[section])
            return self.number_row[section]
        } else {
            return 0;
        }
    }
    override func tableView(_ tableView: UITableView, willDisplayHeaderView view: UIView, forSection section: Int) {
        print(section)
        let header = view
        
//        header.backgroundColor = UIColor(red:0.56, green:0.74, blue:0.56, alpha:1.0)
        header.tintColor = UIColor.black
//        if let viewWithTag = self.view.viewWithTag(section) {
//            viewWithTag.removeFromSuperview()
//        }
//        let headerFrame = self.view.frame.size
//        let theImageView = UIImageView(frame: CGRect(x: headerFrame.width - 32, y: 13, width: 18, height: 18));
//        theImageView.image = UIImage(named: "right")
////        theImageView.tag = section
//        header.addSubview(theImageView)

        header.tag = section
        let headerTapGesture = UITapGestureRecognizer()
        headerTapGesture.addTarget(self, action: #selector(HistoryTableViewController.sectionHeaderWasTouched(_:)))
        header.addGestureRecognizer(headerTapGesture)
    }
    @objc func sectionHeaderWasTouched(_ sender: UITapGestureRecognizer) {
        let headerView = sender.view
        
        let section    = headerView?.tag
//                let headerFrame = self.view.frame.size
//                let theImageView = UIImageView(frame: CGRect(x: headerFrame.width - 32, y: 13, width: 18, height: 18));
//                theImageView.image = UIImage(named: "down")
//                theImageView.tag = section!
//                headerView!.addSubview(theImageView)
        expandedSectionHeaderNumber=section!
        loadView()
    }
    override func tableView(_ tableView: UITableView, titleForHeaderInSection section: Int) -> String? {
//        var keys = Array(self.date1.keys)
//        print(keys)
        return self.new_data[section]
    }
    override func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        let returnedView = UIView(frame: CGRect(x: 0, y: 0, width: view.frame.size.width, height: 25))
//        returnedView.backgroundColor = UIColor.init(red: 109, green: 130, blue: 100, alpha: 1)
        returnedView.backgroundColor = .lightGray
//
        let label = UILabel(frame: CGRect(x: 10, y: 7, width: view.frame.size.width, height: 25))
//        let date = Date()
        let date = self.new_data[section]
        let formatter  = DateFormatter()
        formatter.dateFormat = "yy-MM-dd"
        guard let todayDate = formatter.date(from: date) else { return nil }
        formatter.dateFormat = "EEEE, MMMM dd, yyyy"
        let currentDateString: String = formatter.string(from: todayDate)
        label.text = currentDateString
        print(self.new_data[section])
//        label.textColor = UIColor.init(red: 0, green: 122/255.0, blue: 1, alpha: 1)
//        label.textAlignment = .center
        label.textColor = UIColor.black
        returnedView.addSubview(label)
        return returnedView
    }
    override func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        print(indexPath)
        expandedSectionHeaderNumber=indexPath.section
        loadView()
        
    }
    
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "reuseIdentifier", for: indexPath)
        var fanArr = [String]()
        var lightArr = [String]()
        var time = [String]()
        var keys = Array(self.date1.keys.sorted().reversed())
        let data=self.date1[keys[indexPath.section]]
        for (key,value) in data!.sorted(by: {$0.0 < $1.0}).reversed(){
            fanArr.append(value["fan"]!)
            lightArr.append(value["light"]!)
            time.append(key)
        }
        
        cell.textLabel?.text=time[indexPath.row]
        if fanArr[indexPath.row] == "Nil"{
            cell.detailTextLabel?.text="       Light "+lightArr[indexPath.row]
        }else if lightArr[indexPath.row] == "Nil"{
            cell.detailTextLabel?.text="Fan "+fanArr[indexPath.row]+"         "

        }else{
            cell.detailTextLabel?.text="Fan "+fanArr[indexPath.row] + " Light " + lightArr[indexPath.row]
        }
        return cell
    }
    /*
     Override to support conditional editing of the table view.
    override func tableView(_ tableView: UITableView, canEditRowAt indexPath: IndexPath) -> Bool {
         Return false if you do not want the specified item to be editable.
        return true
    }
    */

    /*
    // Override to support editing the table view.
    override func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCellEditingStyle, forRowAt indexPath: IndexPath) {
        if editingStyle == .delete {
            // Delete the row from the data source
            tableView.deleteRows(at: [indexPath], with: .fade)
        } else if editingStyle == .insert {
            // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
        }    
    }
    */

    /*
    // Override to support rearranging the table view.
    override func tableView(_ tableView: UITableView, moveRowAt fromIndexPath: IndexPath, to: IndexPath) {

    }
    */

    /*
    // Override to support conditional rearranging of the table view.
    override func tableView(_ tableView: UITableView, canMoveRowAt indexPath: IndexPath) -> Bool {
        // Return false if you do not want the item to be re-orderable.
        return true
    }
    */

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
    }
    */

}
